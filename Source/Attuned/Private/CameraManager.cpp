// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraManager.h"
#include "Engine/GameEngine.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCameraManager::UCameraManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	mv_initialized = false;
	mv_debug = false;
}

void UCameraManager::Initialize(USpringArmComponent* CameraBoom, UCameraComponent* FollowCamera, USphereComponent* CameraCollision)
{
	mc_CameraBoom = CameraBoom;
	mc_FollowCamera = FollowCamera;
	mc_CameraCollision = CameraCollision;

	mc_CameraBoom->bDoCollisionTest = false;
	mc_CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	mc_CameraBoom->TargetArmLength = 500.0f;

	mc_CameraCollision->SetSphereRadius(40.f),

	mv_MinPitch = -0.5f;
	mv_MaxPitch = 0.9f;

	mv_MaxArmLength = mc_CameraBoom->TargetArmLength;
	mv_initialized = true;
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

	TargetPos = mc_CameraBoom->GetComponentLocation();
	FakeArmLength = mv_MaxArmLength;

	FakeCameraArm = mc_FollowCamera->GetForwardVector();
	FakeCameraArm.Normalize();
	FakeCameraArm *= FakeArmLength;

	Pitch = ((TargetPos - FakeCameraArm).Z - mc_CameraBoom->GetComponentLocation().Z) / FakeArmLength;

	mv_CurrentPitch = Pitch;
}

float UCameraManager::getPercentBetweenAB(float x, float a, float b)
{
	return ((x - a) / (b - a));
}

void UCameraManager::UpdateCameraFromPitch(void)
{
	static const float BaseCameraBoomZ(60.f), DeltaCameraBoomZ(40.f);
	static const float BaseFOV(90.f), DeltaFOV(10.f);

	this->UpdatePitch();

	//The curve is between 0 and 2 and the pitch is between -1 and 1
	mv_MaxArmLengthFromPitch = ArmLengthCurveFromPitch->GetFloatValue(mv_CurrentPitch + 1) * mv_MaxArmLength;
	
	mc_CameraBoom->SetRelativeLocation(
		FVector(0.f, 0.f,
			BaseCameraBoomZ + FMath::Lerp(
				DeltaCameraBoomZ,
				0.f,
				this->getPercentBetweenAB(mv_CurrentPitch, mv_MinPitch, mv_MaxPitch))));
	
	mc_FollowCamera->SetFieldOfView(
		BaseFOV + FMath::Lerp(
			DeltaFOV,
			0.f, 
			this->getPercentBetweenAB(mv_CurrentPitch, mv_MinPitch, mv_MaxPitch)));

	mv_NextCameraLocation = mc_FollowCamera->GetForwardVector();
	mv_NextCameraLocation.Normalize();
	mv_NextCameraLocation *= mv_MaxArmLengthFromPitch;
	mv_NextCameraLocation = mc_CameraBoom->GetComponentLocation() - mv_NextCameraLocation;
}

void	UCameraManager::CollisionBetweenCameraAndTarget(void)
{
	FHitResult				RV_Hit(ForceInit);
	static bool				success;
	static TArray<AActor*>	FoundActors;

	//Raycast from Target to Camera
	success = GetWorld()->LineTraceSingleByChannel(
		RV_Hit,									 //result
		mc_CameraBoom->GetComponentLocation(),   //start
		mv_NextCameraLocation,					 //end
		ECC_Camera,								 //collision channel
		mv_RV_TraceParams						 //params
	);

	if (!success)
	{
		mc_CameraCollision->SetWorldLocation(mv_NextCameraLocation);
		mc_CameraCollision->GetOverlappingActors(FoundActors, nullptr);
		if (FoundActors.Num() > 0)
		{
			success = GetWorld()->LineTraceSingleByChannel(
				RV_Hit,									 //result
				mc_CameraBoom->GetComponentLocation(),   //start
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
		mc_CameraBoom->TargetArmLength = this->GetArmLength(mv_NextArmLength);
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

		mc_CameraBoom->TargetArmLength = this->GetArmLength(ZoomingOutArmLength);
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
		mc_CameraBoom->TargetArmLength = this->GetArmLength(mv_NextArmLength);
		LastArmLength = mv_NextArmLength;
	}
}

// Called every frame
void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	mv_DeltaTime = DeltaTime;

	if (!mv_initialized)
		return;

	this->UpdateCameraFromPitch();
	this->CollisionBetweenCameraAndTarget();
	this->ZoomOut();
}

