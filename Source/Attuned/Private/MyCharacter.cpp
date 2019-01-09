// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DestructibleActor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathVectorCommon.h"


//////////////////////////////////////////////////////////////////////////
// AMyCharacter

AMyCharacter::AMyCharacter()
{
	//counter to refresh Debug GUI text IG
	mv_DebugFlushTime = 0.f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 20.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
		//Rot
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

		//Jump
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->BrakingDecelerationFalling = 0.5f;
	GetCharacterMovement()->FallingLateralFriction = 0.5f;

		//Walk
	GetCharacterMovement()->MaxAcceleration = 500.f;
	GetCharacterMovement()->MaxStepHeight = 50.f;
	GetCharacterMovement()->SetWalkableFloorAngle(90.f);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create Camera Collision
	CameraCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CameraCollision"));
	CameraCollision->SetupAttachment(FollowCamera);

	// Create Renderer Text - MoveSpeed
	mc_MoveSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MoveSpeed"), true);
	mc_MoveSpeed->SetupAttachment(RootComponent);

	mc_MoveSpeed->SetWorldLocation(FVector(-90.f, 0.f, -30.f));
	mc_MoveSpeed->SetWorldRotation(FRotator(0.f, -180.f, 0.f));
	mc_MoveSpeed->SetHorizontalAlignment(EHTA_Center);
	mc_MoveSpeed->SetVerticalAlignment(EVRTA_TextCenter);
	mc_MoveSpeed->SetTextRenderColor(FColor(0, 172, 47, 255));
	mc_MoveSpeed->SetWorldSize(20);
	mc_MoveSpeed->SetText(FString("MoveSpeed: ") + FString::SanitizeFloat(FVector(this->GetVelocity()).Size()));

	// Create Renderer Text - JumpSpeed
	mc_JumpSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("JumpSpeed"), true);
	mc_JumpSpeed->SetupAttachment(RootComponent);

	mc_JumpSpeed->SetWorldLocation(FVector(-90.f, 0.f, -45.f));
	mc_JumpSpeed->SetWorldRotation(FRotator(0.f, -180.f, 0.f));
	mc_JumpSpeed->SetHorizontalAlignment(EHTA_Center);
	mc_JumpSpeed->SetVerticalAlignment(EVRTA_TextCenter);
	mc_JumpSpeed->SetTextRenderColor(FColor(172, 0, 47, 255));
	mc_JumpSpeed->SetWorldSize(20);
	mc_JumpSpeed->SetText(FString("JumpSpeed: ") + FString::SanitizeFloat(FVector(this->GetVelocity()).Size()));

	// Create TerrainManager
	mc_TerrainManager = CreateDefaultSubobject<UTerrainManager>(TEXT("TerrainManager"));

	// Create CameraManager
	mc_CameraManager = CreateDefaultSubobject<UCameraManager>(TEXT("CameraManager"));
	mc_CameraManager->Initialize(CameraBoom, FollowCamera, CameraCollision);

	// Create RadialDashForce
	mc_DashRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("DashRadialImpulse"));
	mc_DashRadialForce->SetupAttachment(RootComponent);
	mc_DashRadialForce->SetRelativeLocation(FVector(50.f, 0.f, 0.f));
	mc_DashRadialForce->Radius = 50.f;
	mc_DashRadialForce->DestructibleDamage = 2.f;

	// isDashingValue for AnimBlueprint
	mv_isDashing = false;

	//If Controllers are locked
	mv_LockControls = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (mc_InGameUIC) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		mc_InGameUIAttached = CreateWidget<UW_InGameUI>(GetWorld(), mc_InGameUIC);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (mc_InGameUIAttached)
		{
			//let add it to the view port
			mc_InGameUIAttached->AddToViewport();
			mc_InGameUIAttached->SetCoolDown(100.f);
			mc_InGameUIAttached->SetVisibility(ESlateVisibility::Hidden);
			mc_TerrainManager->mc_InGameUIAttached = mc_InGameUIAttached;
		}
	}

}

void AMyCharacter::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	mv_DeltaTime = DeltaTime;
	mv_DebugFlushTime += DeltaTime;

	//Update Debug 3D Text each 0.2sec
	if (mv_DebugFlushTime > 0.2f)
	{
		mv_DebugFlushTime = 0.f;
		mc_MoveSpeed->SetText(FString("MoveSpeed: ") + FString::SanitizeFloat(FVector(this->GetVelocity()).Size()));
		mc_JumpSpeed->SetText(FString("JumpSpeed: ") + FString::SanitizeFloat(this->GetVelocity().Z));
	}
	//Dash
	if (mv_isDashing)
	{
		this->Dash(false);
	}
	//Update Debug 3D Text each frame
	this->UpdateDebugTextLocation();

	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::SanitizeFloat(this->FollowCamera->GetComponentRotation().Yaw));
}

void AMyCharacter::Dash(const bool InitDash)
{
	static float		CurrentDashDuration(0.f); //CurrentTimeValue Needed for the lerp (blueprint timeline like)
	static const float	MaxDashDuration(0.2f); //The Duration of a dash in seconds

	static FVector	NormalizedNormalRamp(0.f, 0.f, 0.f); //The normal of terrain where the player is at the start
	static FVector	BasePlayerLocation(0.f, 0.f, 0.f); //Start location of player
	static FVector	NextLocation(0.f, 0.f, 0.f); //Next location computed each frame
	static FVector	A; //lerp A
	static FVector	B; // lerp B
	static FVector	DashVelocity; //Velocity to not stop movements after dashing
	
	static TArray<TEnumAsByte<EObjectTypeQuery>>	ObjectTypes; //Types of object to focus on for the collision detection
	static TArray<AActor*>							ActorsToIgnore; //Actors to not fetch during the dash collision detection
	static TArray<AActor*>							FoundDestructibleActors; //All destructibleActors in the current world
	static TArray<UPrimitiveComponent*>				OutComponents; // Components detected during a frame for the next dash position

	if (InitDash == true && mc_TerrainManager->mv_CanDash)
	{
		//speed forward TODO
		mv_isDashing = true;
		mv_LockControls = true;
		BasePlayerLocation = this->GetActorLocation();
		NormalizedNormalRamp = mc_TerrainManager->mv_TerrainNormal;
		CurrentDashDuration = 0.f;

		A = BasePlayerLocation;
		B = NormalizedNormalRamp;

		B = B.RotateAngleAxis(90.f, FVector(1.f, 0.f, 0.f)) * 1000.f;
		B = B.RotateAngleAxis(GetCapsuleComponent()->GetComponentRotation().Yaw + 90.f, NormalizedNormalRamp);
		B += A;

		DashVelocity = (B - A);
		DashVelocity.Normalize();
		DashVelocity *= 1000.f;

		if (ObjectTypes.Num() == 0)
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		if (ActorsToIgnore.Num() == 0)
		{
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleActor::StaticClass(), FoundDestructibleActors);
			ActorsToIgnore.Add(GetOwner());
			ActorsToIgnore.Append(FoundDestructibleActors);
		}
		mc_TerrainManager->DashCoolDown(true);
	}
	
	CurrentDashDuration += mv_DeltaTime;
	if (CurrentDashDuration > MaxDashDuration)
		CurrentDashDuration = MaxDashDuration;

	NextLocation = FMath::Lerp(
		A,
		B,
		CurrentDashDuration / MaxDashDuration);

	GetCharacterMovement()->Velocity = DashVelocity;

	if (UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), NextLocation, 10.f, ObjectTypes, nullptr, ActorsToIgnore, OutComponents))
	{
		CurrentDashDuration = MaxDashDuration;
	}
	else
	{
		SetActorLocation(NextLocation);
		mc_DashRadialForce->FireImpulse();
	}

	if (CurrentDashDuration == MaxDashDuration)
	{
		mv_isDashing = false;
		mv_LockControls = false;
	}
}

void AMyCharacter::Jump()
{
	if (this->getTerrainSurfaceType() == TEXT("ROCK") && !mv_isDashing)
	{
		this->Dash(true);
	}
	else
	{
		ACharacter::Jump();
	}
}

void AMyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::LookUpAtRate(float Rate)
{
	// Pitch Locked between Min and Max Pitch
	if (!(((mc_CameraManager->mv_CurrentPitch <= mc_CameraManager->mv_MinPitch) && Rate < 0.f) ||
		((mc_CameraManager->mv_CurrentPitch >= mc_CameraManager->mv_MaxPitch) && Rate > 0.f)))
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMyCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !mv_LockControls)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !mv_LockControls)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

FString AMyCharacter::getTerrainSurfaceType(void)
{
	return (mc_TerrainManager->mv_TerrainType);
}

void AMyCharacter::UpdateDebugTextLocation(void)
{
	FVector UIOnPlayerPos;

	UIOnPlayerPos = UKismetMathLibrary::GetForwardVector(Controller->GetControlRotation());
	UIOnPlayerPos.Normalize();
	UIOnPlayerPos *= -100.f;

	UIOnPlayerPos = FVector(UIOnPlayerPos.X, UIOnPlayerPos.Y, -30.f);
	mc_MoveSpeed->SetWorldLocation(this->GetCapsuleComponent()->GetComponentLocation() + UIOnPlayerPos);
	mc_MoveSpeed->SetWorldRotation(FRotator(0.f, Controller->GetControlRotation().Yaw + 180.f, 0.f));

	UIOnPlayerPos = FVector(UIOnPlayerPos.X, UIOnPlayerPos.Y, -45.f);
	mc_JumpSpeed->SetWorldLocation(this->GetCapsuleComponent()->GetComponentLocation() + UIOnPlayerPos);
	mc_JumpSpeed->SetWorldRotation(FRotator(0.f, Controller->GetControlRotation().Yaw + 180.f, 0.f));
}