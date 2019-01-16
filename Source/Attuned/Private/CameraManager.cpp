// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraManager.h"
#include "MyCharacter.h"
#include "Engine/GameEngine.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UCameraManager::UCameraManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	mv_initialized = false;
	mv_debug = false;
}

float UCameraManager::GetArmLength(float ArmLength) const
{
	return (ArmLength - mc_character->mc_CurrentCameraCollision->GetScaledSphereRadius());
}

void UCameraManager::Initialize(void)
{
	//DEFAULT CAM

	mc_character->mc_DefaultCameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	mc_character->mc_DefaultCameraBoom->bDoCollisionTest = false;
	mc_character->mc_DefaultCameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	mc_character->mc_DefaultCameraBoom->TargetArmLength = 500.0f;

	mc_character->mc_DefaultFollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	mc_character->mc_DefaultCameraCollision->SetSphereRadius(40.f),

	//WATER CAM

	mc_character->mc_WaterCameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	mc_character->mc_WaterCameraBoom->bEnableCameraRotationLag = true;
	mc_character->mc_WaterCameraBoom->CameraRotationLagSpeed = 7.f;
	mc_character->mc_WaterCameraBoom->bInheritPitch = false;
	mc_character->mc_WaterCameraBoom->bInheritRoll = false;
	mc_character->mc_WaterCameraBoom->TargetArmLength = 600.f;
	mc_character->mc_WaterCameraBoom->SetRelativeLocation(FVector(7.f, 0.f, 40.f));
	mc_character->mc_WaterCameraBoom->RelativeRotation = FRotator(0.f, 10.f, 0.f);

	mc_character->mc_WaterFollowCamera->Deactivate();

	mc_character->mc_WaterCameraCollision->SetSphereRadius(40.f),
	

	//GLOBAL PARAMETERS

	mv_MinPitch = -0.5f;
	mv_MaxPitch = 0.9f;

	mv_MaxArmLength = mc_character->mc_CurrentCameraBoom->TargetArmLength;

	UE_LOG(LogTemp, Warning, TEXT("Dont forget to set ArmLengthCurveFromPitch"));

	mv_initialized = true;
}

void	UCameraManager::SetOwner(AMyCharacter* owner)
{
	mc_character = owner;
	this->Initialize();
}

// Called when the game starts
void UCameraManager::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	const FName TraceTag("CameraTraceTag");

	GetWorld()->DebugDrawTraceTag = TraceTag;
	mv_RV_TraceParams.TraceTag = TraceTag;
	*/
}

//Get The current pitch between -1 and 1
void UCameraManager::UpdatePitch(void)
{
	static FVector	FakeCameraArm, TargetPos;
	static float	FakeArmLength;
	static float	Pitch;

	TargetPos = mc_character->mc_CurrentCameraBoom->GetComponentLocation();
	FakeArmLength = mv_MaxArmLength;

	FakeCameraArm = mc_character->mc_CurrentFollowCamera->GetForwardVector();
	FakeCameraArm.Normalize();
	FakeCameraArm *= FakeArmLength;

	Pitch = ((TargetPos - FakeCameraArm).Z - mc_character->mc_CurrentCameraBoom->GetComponentLocation().Z) / FakeArmLength;
	mv_CurrentPitch = Pitch;
}

float UCameraManager::GetPercentBetweenAB(float x, float a, float b)
{
	return ((x - a) / (b - a));
}

void UCameraManager::UpdateCameraFromPitch(void)
{
	static const float BaseCameraBoomZ(60.f), DeltaCameraBoomZ(40.f);
	static const float BaseFOV(90.f), DeltaFOV(10.f);

	this->UpdatePitch();

	//The curve is between 0 and 2 and the pitch is between -1 and 1
	mv_MaxArmLengthFromPitch = mc_ArmLengthCurveFromPitch->GetFloatValue(mv_CurrentPitch + 1) * mv_MaxArmLength;

	mc_character->mc_CurrentCameraBoom->SetRelativeLocation(
		FVector(0.f, 0.f,
			BaseCameraBoomZ + FMath::Lerp(
				DeltaCameraBoomZ,
				0.f,
				this->GetPercentBetweenAB(mv_CurrentPitch, mv_MinPitch, mv_MaxPitch))));

	mc_character->mc_CurrentFollowCamera->SetFieldOfView(
		BaseFOV + FMath::Lerp(
			DeltaFOV,
			0.f, 
			this->GetPercentBetweenAB(mv_CurrentPitch, mv_MinPitch, mv_MaxPitch)));

	mv_NextCameraLocation = mc_character->mc_CurrentFollowCamera->GetForwardVector();
	mv_NextCameraLocation.Normalize();
	mv_NextCameraLocation *= mv_MaxArmLengthFromPitch;
	mv_NextCameraLocation = mc_character->mc_CurrentCameraBoom->GetComponentLocation() - mv_NextCameraLocation;
}

void	UCameraManager::CollisionBetweenCameraAndTarget(void)
{
	FHitResult				RV_Hit(ForceInit);
	static bool				success;
	static TArray<AActor*>	FoundActors;

	//Raycast from Target to Camera
	success = GetWorld()->LineTraceSingleByChannel(
		RV_Hit,									 //result
		mc_character->mc_CurrentCameraBoom->GetComponentLocation(),   //start
		mv_NextCameraLocation,					 //end
		ECC_Camera,								 //collision channel
		mv_RV_TraceParams						 //params
	);

	if (!success)
	{
		mc_character->mc_CurrentCameraCollision->SetWorldLocation(mv_NextCameraLocation);
		mc_character->mc_CurrentCameraCollision->GetOverlappingActors(FoundActors, nullptr);
		if (FoundActors.Num() > 0)
		{
			success = GetWorld()->LineTraceSingleByChannel(
				RV_Hit,									 //result
				mc_character->mc_CurrentCameraBoom->GetComponentLocation(),   //start
				mv_NextCameraLocation,					 //end
				ECC_Visibility,							 //collision channel
				mv_RV_TraceParams						 //params
			);
			if (!success)
			{
				mv_NextArmLength = mv_MaxArmLengthFromPitch;
				return;
			}
			mv_NextArmLength = RV_Hit.Distance;
		}
		else
		{
			mv_NextArmLength = mv_MaxArmLengthFromPitch;
		}
		return;
	}

	if ((RV_Hit.Distance > 0.f) &&
		(RV_Hit.Distance < mv_MaxArmLengthFromPitch))
	{
		mv_NextArmLength = RV_Hit.Distance;
	}
}

void UCameraManager::ZoomOut(void)
{
	static const float	ZoomOutMaxDuration = 2.f;
	static float		ZoomOutCurrentDuration = 0.f;
	static float		LastArmLength = 500.f;
	static bool			IsZoomingOut = false;

	if ((LastArmLength > mv_NextArmLength)
		&& (mv_NextArmLength < mv_MaxArmLengthFromPitch))
	{
		IsZoomingOut = true;
		ZoomOutCurrentDuration = (mv_NextArmLength / mv_MaxArmLengthFromPitch) * ZoomOutMaxDuration;
		mc_character->mc_CurrentCameraBoom->TargetArmLength = this->GetArmLength(mv_NextArmLength);
		LastArmLength = mv_NextArmLength;
		if (GEngine && mv_debug)
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString("INIT ZOOM Out : ") + FString::SanitizeFloat(mv_NextArmLength) + FString("; ") + FString::SanitizeFloat(mv_MaxArmLengthFromPitch));
		return;
	}

	if (IsZoomingOut
		&& (LastArmLength != mv_NextArmLength))
	{
		ZoomOutCurrentDuration += mv_DeltaTime;
		if (ZoomOutCurrentDuration > ZoomOutMaxDuration)
			ZoomOutCurrentDuration = ZoomOutMaxDuration;

		const float	ZoomingOutArmLength(FMath::Lerp(0.f, 1.f, ZoomOutCurrentDuration / ZoomOutMaxDuration) * mv_MaxArmLengthFromPitch);

		mc_character->mc_CurrentCameraBoom->TargetArmLength = this->GetArmLength(ZoomingOutArmLength);
		LastArmLength = ZoomingOutArmLength;

		if (ZoomOutCurrentDuration == ZoomOutMaxDuration)
		{
			if (GEngine && mv_debug)
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString("END Zooming Out"));
			IsZoomingOut = false;
		}

		if (GEngine && mv_debug)
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString("Zooming Out: ") + FString::SanitizeFloat(ZoomOutCurrentDuration / ZoomOutMaxDuration));
	}
	else
	{
		mc_character->mc_CurrentCameraBoom->TargetArmLength = this->GetArmLength(mv_NextArmLength);
		LastArmLength = mv_NextArmLength;
	}
}

void UCameraManager::UpdateArmFromSpeed(void)
{
	static const float WaterArmLength = 600.f;

	mc_character->mc_CurrentCameraBoom->TargetArmLength = FMath::Lerp(
		400.f,
		WaterArmLength,
		mc_character->GetVelocity().Size() / mc_character->mc_TerrainManager->mv_WaterSpeed);
}

// Called every frame
void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	mv_DeltaTime = DeltaTime;

	if (!mv_initialized)
		return;

	if (mc_character->GetTerrainSurfaceType() != "WATER")
	{
		this->UpdateCameraFromPitch();
		this->CollisionBetweenCameraAndTarget();
		this->ZoomOut();
	}
	else
	{
		this->UpdateArmFromSpeed();
	}
}

