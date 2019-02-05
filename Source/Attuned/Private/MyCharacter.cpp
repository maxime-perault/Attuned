// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyCharacter.h"
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
#include "Math/UnrealMathUtility.h"


//////////////////////////////////////////////////////////////////////////
// AMyCharacter

AMyCharacter::AMyCharacter()
{
	//Lean percent for 4D blend anim
	mv_LeanPercent = 0.f;

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
	

	/*
	** CAMERAS
	*/

		/*
		** DEFAULT CAM
		*/

			// Create a camera boom (pulls in towards the player if there is a collision)
			mc_DefaultCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("DefaultCameraBoom"));
			mc_DefaultCameraBoom->SetupAttachment(RootComponent);

			// Create a follow camera
			mc_DefaultFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DefaultFollowCamera"));
			mc_DefaultFollowCamera->SetupAttachment(mc_DefaultCameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
			

			// Create Camera Collision
			mc_DefaultCameraCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DefaultCameraCollision"));
			mc_DefaultCameraCollision->SetupAttachment(mc_DefaultFollowCamera);

		/*
		** WATER CAM
		*/
			// Create a camera boom (pulls in towards the player if there is a collision)
			mc_WaterCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("WaterCameraBoom"));
			mc_WaterCameraBoom->SetupAttachment(RootComponent);
			
			// Create a follow camera
			mc_WaterFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("WaterFollowCamera"));
			mc_WaterFollowCamera->SetupAttachment(mc_WaterCameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation

			// Create Camera Collision
			mc_WaterCameraCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WaterCameraCollision"));
			mc_WaterCameraCollision->SetupAttachment(mc_WaterFollowCamera);

		/*
		** CURRENT CAM
		*/
			mc_CurrentCameraBoom = mc_DefaultCameraBoom;
			mc_CurrentFollowCamera = mc_DefaultFollowCamera;
			mc_CurrentCameraCollision = mc_DefaultCameraCollision;

	/*
	** !CAMERAS
	*/

	// Create Renderer Text - MoveSpeed
	mc_MoveSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MoveSpeed"), true);
	mc_MoveSpeed->SetupAttachment(RootComponent);

	mc_MoveSpeed->SetWorldLocation(FVector(-90.f, 0.f, -30.f));
	mc_MoveSpeed->SetWorldRotation(FRotator(0.f, -180.f, 0.f));
	mc_MoveSpeed->SetHorizontalAlignment(EHTA_Center);
	mc_MoveSpeed->SetVerticalAlignment(EVRTA_TextCenter);
	mc_MoveSpeed->SetTextRenderColor(FColor(0, 172, 47, 255));
	mc_MoveSpeed->SetWorldSize(20);
	mc_MoveSpeed->SetText(FString("MoveSpeed: ") + FString::SanitizeFloat(this->GetVelocity().Size()));

	// Create Renderer Text - JumpSpeed
	mc_JumpSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("JumpSpeed"), true);
	mc_JumpSpeed->SetupAttachment(RootComponent);

	mc_JumpSpeed->SetWorldLocation(FVector(-90.f, 0.f, -45.f));
	mc_JumpSpeed->SetWorldRotation(FRotator(0.f, -180.f, 0.f));
	mc_JumpSpeed->SetHorizontalAlignment(EHTA_Center);
	mc_JumpSpeed->SetVerticalAlignment(EVRTA_TextCenter);
	mc_JumpSpeed->SetTextRenderColor(FColor(172, 0, 47, 255));
	mc_JumpSpeed->SetWorldSize(20);
	mc_JumpSpeed->SetText(FString("JumpSpeed: ") + FString::SanitizeFloat(this->GetVelocity().Size()));

	// Create RadialDashForce
	mc_DashRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("DashRadialImpulse"));
	mc_DashRadialForce->SetupAttachment(RootComponent);
	mc_DashRadialForce->SetRelativeLocation(FVector(50.f, 0.f, 0.f));
	mc_DashRadialForce->Radius = 50.f;
	mc_DashRadialForce->DestructibleDamage = 2.f;

	// Create RadialJumpForce
	mc_JumpRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("JumpRadialImpulse"));
	mc_JumpRadialForce->SetupAttachment(RootComponent);
	mc_JumpRadialForce->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	mc_JumpRadialForce->Radius = 50.f;
	mc_JumpRadialForce->DestructibleDamage = 2.f;

	// isDashingValue for AnimBlueprint
	mv_isDashing = false;

	//If Controllers are locked
	mv_LockControls = true;
	mv_CanPlay = false;
	mv_StartPlay = false;

	// Create TerrainManager
	mc_TerrainManager = CreateDefaultSubobject<UTerrainManager>(TEXT("TerrainManager"));
	mc_TerrainManager->SetOwner(this);

	// Create CameraManager
	mc_CameraManager = CreateDefaultSubobject<UCameraManager>(TEXT("CameraManager"));
	mc_CameraManager->SetOwner(this);

	mv_DrawSpeedParticles = false;

	mv_MenuPosition = 0;

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
	mc_CurrentCameraBoom = mc_DefaultCameraBoom;
	mc_CurrentFollowCamera = mc_DefaultFollowCamera;
	mc_CurrentCameraCollision = mc_DefaultCameraCollision;
	mv_ForwardSpeed = 0.f;
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
}

void AMyCharacter::Dash(const bool InitDash)
{
	static float			CurrentDashDuration(0.f); //CurrentTimeValue Needed for the lerp (blueprint timeline like)
	static const float		MaxDashDuration(0.2f); //The Duration of a dash in seconds
	static const float		DashLength(1000.f);

	static FVector	NormalizedForward(0.f, 0.f, 0.f);
	
	static TArray<TEnumAsByte<EObjectTypeQuery>>	ObjectTypes; //Types of object to focus on for the collision detection
	static TArray<AActor*>							ActorsToIgnore; //Actors to not fetch during the dash collision detection
	static TArray<AActor*>							FoundDestructibleActors; //All destructibleActors in the current world
	static TArray<UPrimitiveComponent*>				OutComponents; // Components detected during a frame for the next dash position

	if (InitDash == true && mc_TerrainManager->mv_CanDash)
	{
		//speed forward TODO
		mv_isDashing = true;
		mv_LockControls = true;
		CurrentDashDuration = 0.f;

		if (ObjectTypes.Num() == 0)
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		if (ActorsToIgnore.Num() == 0)
		{
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleActor::StaticClass(), FoundDestructibleActors);
			ActorsToIgnore.Add(GetOwner());
			ActorsToIgnore.Append(FoundDestructibleActors);
		}
		NormalizedForward = GetCapsuleComponent()->GetForwardVector();
		NormalizedForward.Normalize();

		mc_TerrainManager->DashCoolDown(true);
		GetCharacterMovement()->AddImpulse(NormalizedForward * mv_DashPower, true);
	}
	
	CurrentDashDuration += mv_DeltaTime;


	if (CurrentDashDuration > MaxDashDuration)
		CurrentDashDuration = MaxDashDuration;
	
	if (UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), mc_DashRadialForce->GetComponentLocation(), 10.f, ObjectTypes, nullptr, ActorsToIgnore, OutComponents))
	{
		CurrentDashDuration = MaxDashDuration;
	}
	else if (CurrentDashDuration != MaxDashDuration)
	{
		mc_DashRadialForce->FireImpulse();
	}

	if (CurrentDashDuration == MaxDashDuration)
	{
		mv_isDashing = false;
		mv_LockControls = false;
		GetCharacterMovement()->Velocity = NormalizedForward * 1425.f;
	}
}

void AMyCharacter::Jump()
{
	if (!mv_StartPlay && (mv_MenuPosition == 0))
		mv_StartPlay = true;
	
	if (!mv_CanPlay)
		return;

	if ((this->GetTerrainSurfaceType() == TEXT("ROCK")) &&
		!mv_isDashing &&
		(GetCharacterMovement()->IsFalling() == false))
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
	if (!mv_LockControls)
	{
		if (mc_TerrainManager->mv_TerrainType != "WATER")
		{
			AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		}
	}
}

void AMyCharacter::LookUpAtRate(float Rate)
{
	// Pitch Locked between Min and Max Pitch
	if (!mv_LockControls)
	{
		if (mc_TerrainManager->mv_TerrainType != "WATER")
		{
			if (!(((mc_CameraManager->mv_CurrentPitch <= mc_CameraManager->mv_MinPitch) && Rate < 0.f) ||
				((mc_CameraManager->mv_CurrentPitch >= mc_CameraManager->mv_MaxPitch) && Rate > 0.f)))
			{
				AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
			}
		}
	}
}

void AMyCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && !mv_LockControls)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (mc_TerrainManager->mv_TerrainType != "WATER")
		{
			AddMovementInput(Direction, Value);
		}
		else
		{
			const float	water_value(FMath::Clamp(Value, 0.f, 1.f));

			mv_ForwardSpeed = water_value;
		}
	}
}

float	AMyCharacter::LerpForwardSpeed(const float NewSpeed, const float DeltaTime, const bool reset)
{
	static const float TotalTime = 1.f;
	static float CurrentTime = 0.f;
	static float CurrentSpeed = 0.f;

	if (reset)
	{
		CurrentSpeed = NewSpeed;
		CurrentTime = NewSpeed * TotalTime;
		return (CurrentSpeed);
	}

	if ((NewSpeed > CurrentSpeed) && (CurrentSpeed < 1.f))
	{
		CurrentTime += DeltaTime;
		CurrentSpeed = FMath::Lerp(0.f, 1.f, CurrentTime / TotalTime);
		if (CurrentSpeed > 1.f)
			CurrentSpeed = 1.f;
		return (CurrentSpeed);
	}
	else if ((NewSpeed < CurrentSpeed) && (CurrentSpeed > 0.f))
	{
		CurrentTime -= DeltaTime;
		CurrentSpeed = FMath::Lerp(0.f, 1.f, CurrentTime / TotalTime);
		if (CurrentSpeed < 0.f)
			CurrentSpeed = 0.f;
		return (CurrentSpeed);
	}
	return (CurrentSpeed);
}

void AMyCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && !mv_LockControls)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		float coefficient = 1.0f;
		if (mc_TerrainManager->mv_MomemtumActive && mc_TerrainManager->mv_TerrainType == "ROCK")
		{
			// Normalizing the velocity
			float nVelocity   = GetVelocity().Size() / GetCharacterMovement()->MaxWalkSpeed;
			coefficient       = FMath::Min(mc_TerrainManager->mv_MomemtumMinValue + (1.0f - nVelocity), 1.0f);

			if (mc_TerrainManager->mv_MomemtumSquare)
			{
				coefficient *= coefficient;
			}
		}

		BaseTurnRate = 45.0f * coefficient;
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * coefficient;

		if (mc_TerrainManager->mv_TerrainType != "WATER")
		{
			AddMovementInput(Direction, Value);
		}
		else if ((mv_ForwardSpeed > 0.f) || (Value != 0.f))
		{
			float	water_value(FMath::Clamp(Value, -0.4f, 0.4f));
			float	water_TurnRate(40.f);

			AddMovementInput(Direction, water_value);

			mv_ForwardSpeed = FMath::Max(mv_ForwardSpeed, FMath::Abs(Value) / 1.5f);
			mv_ForwardSpeed = LerpForwardSpeed(mv_ForwardSpeed, mv_DeltaTime, false);
			//lerp speed

			AddMovementInput(
				FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X),
				FMath::Clamp(
					mv_ForwardSpeed,
					0.f,
					1.f));
				
			water_TurnRate = 100.f * (abs(water_value) / 0.4f);

			mv_LeanPercent = water_value / 0.4f;

			AddControllerYawInput(water_value * water_TurnRate * GetWorld()->GetDeltaSeconds());
		}
	}
	else
	{
		mv_LeanPercent = 0.f;
	}
	
	//Smooth Deceleration if high forward speed or if no input has been enter
	if ((this->GetTerrainSurfaceType() == "WATER") &&
		(mv_ForwardSpeed < 0.1f))
	{
		GetCharacterMovement()->BrakingDecelerationWalking = 100.f;
		GetCharacterMovement()->GroundFriction = 0.2f;
	}
	else
	{
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
		GetCharacterMovement()->GroundFriction = 8.f;
	}
}

//return type cant be void cause of UE4 blueprint functions constraints
void	AMyCharacter::UnlockControls(void)
{
	mv_CanPlay = true;
	mv_LockControls = false;
}

void	AMyCharacter::lockControls(void)
{
	float i(1);

	mv_CanPlay = false;
	mv_LockControls = true;
}

FString AMyCharacter::GetTerrainSurfaceType(void)
{
	return (mc_TerrainManager->mv_TerrainType);
}

float AMyCharacter::GetLeanDegree(void)
{
	return (mv_LeanPercent);
}

bool AMyCharacter::GetStartPlay(void)
{
	return (mv_StartPlay);
}

bool AMyCharacter::GetCanPlay(void)
{
	return (mv_CanPlay);
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