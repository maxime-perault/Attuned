// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrainManager.h"
#include "MyCharacter.h"
#include "Public/CollisionQueryParams.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/GameEngine.h"
#include "Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTerrainManager::UTerrainManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	mv_LastSurfaceType = SurfaceType_Default;
	mv_TerrainType = TEXT("DEFAULT");
	mv_initialized = false;

	bOnWaterEnter = false;
	bOnWaterExit  = false;
}

void	UTerrainManager::SetOwner(AMyCharacter* owner)
{
	mc_character = owner;
	mv_initialized = true;
}


// Called when the game starts
void UTerrainManager::BeginPlay()
{
	Super::BeginPlay();

	mv_RV_TraceParams = FCollisionQueryParams(true);
	mv_RV_TraceParams.bTraceComplex = true;
	mv_RV_TraceParams.bTraceAsyncScene = true;
	mv_RV_TraceParams.bReturnPhysicalMaterial = true;

	/*
	const FName TraceTag("TerrainTraceTag");

	GetWorld()->DebugDrawTraceTag = TraceTag;
	mv_RV_TraceParams.TraceTag = TraceTag;
	*/

	mv_CanDash = true;

	mv_MaxSpeed = 800.f;
	mv_IsTransitioningSpeed = false;

	// Vars to Edit in real time
	mv_DefaultJumpZVelocity = 600.f;
	mv_DefaultAcceleration = 500.f;
	mv_DefaultSpeed = 600.f;

	mv_RockJumpZVelocity = 600.f; //Not Used
	mv_RockAcceleration = 200.f;
	mv_RockSpeed = 1000.f;

	mv_WaterJumpZVelocity = 800.f;
	mv_WaterAcceleration = 500.f;
	mv_WaterSpeed = 1425.f;

	mv_SandJumpZVelocity = 1000.f;
	mv_SandAcceleration = 1000.f;
	mv_SandSpeed = 1000.f;

	mv_MomemtumActive   = true;
	mv_MomemtumSquare   = false;
	mv_MomemtumMinValue = 0.5f;


	mv_RockFallingFriction = 0.5f;
	mv_RockAirControl      = 0.5f;
	mv_RockJumpZVelocity   = 300.0f;
	mv_RockAcceleration    = 175.0f;
	mv_RockSpeed           = 1425.0f;

	mv_SandFallingFriction = 400.0f;
	mv_SandAirControl      = 250.0f;
	mv_SandJumpZVelocity   = 1400.0f;
	mv_SandAcceleration    = 925.0f;
	mv_SandSpeed           = 950.0f;

	mv_WaterFallingFriction = 1.0f;
	mv_WaterAirControl      = 1.0f;
	mv_WaterJumpZVelocity   = 200.0f;
	mv_WaterAcceleration    = 300.0f;
	mv_WaterSpeed           = 1425.0f;

	mv_DefaultFallingFriction = 0.5f;
	mv_DefaultAirControl      = 0.5f;
	mv_DefaultJumpZVelocity   = 700.0f;
	mv_DefaultAcceleration    = 850.0f;
	mv_DefaultSpeed           = 825.0f;
}


// Called every frame
void UTerrainManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	mv_DeltaTime = DeltaTime;

	if (!mv_initialized)
		return;

	if (!mv_CanDash)
		this->DashCoolDown(false);

	if (mv_IsTransitioningSpeed)
		this->CharacterMoveSpeedTransition(false);
	
	FHitResult RV_Hit(ForceInit);
	static bool success;

	//Raycast from the middle of player with a (0, 0, 150) vector pointing down.
	success = GetWorld()->LineTraceSingleByChannel(
		RV_Hit,										 //result
		FVector(mc_character->GetActorLocation()),   //start
		FVector(mc_character->GetActorLocation().X, mc_character->GetActorLocation().Y, mc_character->GetActorLocation().Z - 150.f),	//end
		ECC_Visibility,								 //collision channel
		mv_RV_TraceParams							 //params
	);
	if (!success)
		return;

	
	mv_TerrainNormal = RV_Hit.ImpactNormal;
	EPhysicalSurface CurrentSurface = RV_Hit.PhysMaterial.Get()->SurfaceType;

	bOnWaterEnter = false;
	bOnWaterExit  = false;

	if ((mv_LastSurfaceType != CurrentSurface)
		&& CurrentSurface != SurfaceType4) // DestructibleMeshSurface
	{
		if (mv_LastSurfaceType == SurfaceType2)
		{
			bOnWaterExit = true;
		}

		mv_LastSurfaceType = CurrentSurface;

		switch (CurrentSurface)
		{
			case SurfaceType1:
			{
				this->RockTerrainFirstStep();
				break;
			}
			case SurfaceType2:
			{
				this->WaterTerrainFirstStep();
				bOnWaterEnter = true;
				break;
			}
			case SurfaceType3:
			{
				this->SandTerrainFirstStep();
				break;
			}
			default:
			{
				this->StandardTerrainFirstStep();
				break;
			}
		}
	}
}

void UTerrainManager::DashCoolDown(const bool reset)
{
	static float		CurrentTime = 0.f;
	static const float	DashCoolDown = 0.5f;

	if (reset)
	{
		CurrentTime = 0.f;
		mv_CanDash = false;
		mc_InGameUIAttached->SetCoolDown(0.f);
		return;
	}
	CurrentTime += mv_DeltaTime;
	if (CurrentTime > DashCoolDown)
		CurrentTime = DashCoolDown;

	mc_InGameUIAttached->SetCoolDown(CurrentTime / DashCoolDown);

	if (CurrentTime == DashCoolDown)
		mv_CanDash = true;
}

void UTerrainManager::CharacterMoveSpeedTransition(const bool InitTransition)
{
	static float		CurrentTime = 0.f;
	static const float	SpeedTransitionTime = 1.5f;
	static float		StartingMaxSpeed = 0.f;

	if (InitTransition)
	{
		CurrentTime = 0.f;
		StartingMaxSpeed = mc_character->GetCharacterMovement()->Velocity.Size();
		mv_IsTransitioningSpeed = true;
	}
	CurrentTime += mv_DeltaTime;

	if (CurrentTime > SpeedTransitionTime)
		CurrentTime = SpeedTransitionTime;

	mc_character->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(StartingMaxSpeed, mv_MaxSpeed, CurrentTime / SpeedTransitionTime);

	if (CurrentTime == SpeedTransitionTime)
		mv_IsTransitioningSpeed = false;
}

void UTerrainManager::StandardTerrainFirstStep(void)
{
	if (mc_InGameUIAttached)
		mc_InGameUIAttached->SetVisibility(ESlateVisibility::Hidden); // If Rock before

	mv_TerrainType = TEXT("DEFAULT");
	mc_character->mc_CameraManager->OnTerrainChange(0);
	mc_character->GetCharacterMovement()->JumpZVelocity = mv_DefaultJumpZVelocity;
	mc_character->GetCharacterMovement()->MaxAcceleration = mv_DefaultAcceleration;

	mc_character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	mc_character->GetCharacterMovement()->bOrientRotationToMovement = true;

	mv_MaxSpeed = mv_DefaultSpeed;

	mc_character->mc_WaterFollowCamera->Deactivate();
	mc_character->mc_DefaultFollowCamera->Activate();

	mc_character->mc_CurrentCameraBoom = mc_character->mc_DefaultCameraBoom;
	mc_character->mc_CurrentFollowCamera = mc_character->mc_DefaultFollowCamera;
	mc_character->mc_CurrentCameraCollision = mc_character->mc_DefaultCameraCollision;

	this->CharacterMoveSpeedTransition(true);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Default"));
}

void UTerrainManager::RockTerrainFirstStep(void)
{
	if (mc_InGameUIAttached)
		mc_InGameUIAttached->SetVisibility(ESlateVisibility::Visible);

	mv_TerrainType = TEXT("ROCK");
	mc_character->mc_CameraManager->OnTerrainChange(1);
	mc_character->GetCharacterMovement()->JumpZVelocity = mv_RockJumpZVelocity; //Not Used
	mc_character->GetCharacterMovement()->MaxAcceleration = mv_RockAcceleration;

	mc_character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	mc_character->GetCharacterMovement()->bOrientRotationToMovement = true;

	mv_MaxSpeed = mv_RockSpeed;

	mc_character->mc_WaterFollowCamera->Deactivate();
	mc_character->mc_DefaultFollowCamera->Activate();

	mc_character->mc_CurrentCameraBoom = mc_character->mc_DefaultCameraBoom;
	mc_character->mc_CurrentFollowCamera = mc_character->mc_DefaultFollowCamera;
	mc_character->mc_CurrentCameraCollision = mc_character->mc_DefaultCameraCollision;

	this->CharacterMoveSpeedTransition(true);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Rock"));
}

void UTerrainManager::WaterTerrainFirstStep(void)
{
	if (mc_InGameUIAttached)
		mc_InGameUIAttached->SetVisibility(ESlateVisibility::Hidden); // If Rock before

	mv_TerrainType = TEXT("WATER");
	mc_character->mc_CameraManager->OnTerrainChange(2);
	mc_character->GetCharacterMovement()->JumpZVelocity = mv_WaterJumpZVelocity;
	mc_character->GetCharacterMovement()->MaxAcceleration = mv_WaterAcceleration;

	mc_character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	mc_character->GetCharacterMovement()->bOrientRotationToMovement = false;
	
	mv_MaxSpeed = mv_WaterSpeed;

	mc_character->mc_DefaultFollowCamera->Deactivate();
	mc_character->mc_WaterFollowCamera->Activate();

	mc_character->mc_CurrentCameraBoom = mc_character->mc_WaterCameraBoom;
	mc_character->mc_CurrentFollowCamera = mc_character->mc_WaterFollowCamera;
	mc_character->mc_CurrentCameraCollision = mc_character->mc_WaterCameraCollision;

	this->CharacterMoveSpeedTransition(true);
	mc_character->LerpForwardSpeed(mc_character->GetVelocity().Size() / mv_WaterSpeed, 0.f, false);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Water"));
}

void UTerrainManager::SandTerrainFirstStep(void)
{
	if (mc_InGameUIAttached)
		mc_InGameUIAttached->SetVisibility(ESlateVisibility::Hidden); // If Rock before

	mv_TerrainType = TEXT("SAND");
	mc_character->mc_CameraManager->OnTerrainChange(3);
	mc_character->GetCharacterMovement()->JumpZVelocity = mv_SandJumpZVelocity;
	mc_character->GetCharacterMovement()->MaxAcceleration = mv_SandAcceleration;

	mc_character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	mc_character->GetCharacterMovement()->bOrientRotationToMovement = true;

	mv_MaxSpeed = mv_SandSpeed;

	mc_character->mc_WaterFollowCamera->Deactivate();
	mc_character->mc_DefaultFollowCamera->Activate();

	mc_character->mc_CurrentCameraBoom = mc_character->mc_DefaultCameraBoom;
	mc_character->mc_CurrentFollowCamera = mc_character->mc_DefaultFollowCamera;
	mc_character->mc_CurrentCameraCollision = mc_character->mc_DefaultCameraCollision;

	this->CharacterMoveSpeedTransition(true);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Sand"));
}

void UTerrainManager::UpdateTerrainSettings(const RockTerrainSettings& settings)
{
	mv_RockSpeed              = settings.RockSpeed;
	mv_RockAirControl         = settings.RockAirControl;
	mv_RockAcceleration       = settings.RockAcceleration;
	mv_RockJumpZVelocity      = settings.RockJumpZVelocity;
	mv_DefaultFallingFriction = settings.RockFallingFriction;
	UpdateCharacterSettings();
}

void UTerrainManager::UpdateTerrainSettings(const SandTerrainSettings& settings)
{
	mv_SandSpeed           = settings.SandSpeed;
	mv_SandAirControl      = settings.SandAirControl;
	mv_SandAcceleration    = settings.SandAcceleration;
	mv_SandJumpZVelocity   = settings.SandJumpZVelocity;
	mv_SandFallingFriction = settings.SandFallingFriction;
	UpdateCharacterSettings();
}

void UTerrainManager::UpdateTerrainSettings(const WaterTerrainSettings& settings)
{
	mv_WaterSpeed           = settings.WaterSpeed;
	mv_WaterAirControl      = settings.WaterAirControl;
	mv_WaterAcceleration    = settings.WaterAcceleration;
	mv_WaterJumpZVelocity   = settings.WaterJumpZVelocity;
	mv_WaterFallingFriction = settings.WaterFallingFriction;
	UpdateCharacterSettings();
}

void UTerrainManager::UpdateTerrainSettings(const NeutralTerrainSettings& settings)
{
	mv_DefaultSpeed           = settings.DefaultSpeed;
	mv_DefaultAirControl      = settings.DefaultAirControl;
	mv_DefaultAcceleration    = settings.DefaultAcceleration;
	mv_DefaultJumpZVelocity   = settings.DefaultJumpZVelocity;
	mv_DefaultFallingFriction = settings.DefaultFallingFriction;
	UpdateCharacterSettings();
}

void UTerrainManager::UpdateCharacterSettings()
{
	float maxSpeed        = 0.0f;
	float airControl      = 0.0f;
	float acceleration    = 0.0f;
	float jumpZVelocity   = 0.0f;
	float fallingFriction = 0.0f;

	if (mv_TerrainType == "ROCK")
	{
		maxSpeed        = mv_RockSpeed;
		airControl      = mv_RockAirControl;
		acceleration    = mv_RockAcceleration;
		jumpZVelocity   = mv_RockJumpZVelocity;
		fallingFriction = mv_RockFallingFriction;
	}
	else if (mv_TerrainType == "SAND")
	{
		maxSpeed        = mv_SandSpeed;
		airControl      = mv_SandAirControl;
		acceleration    = mv_SandAcceleration;
		jumpZVelocity   = mv_SandJumpZVelocity;
		fallingFriction = mv_SandFallingFriction;
	}
	else if (mv_TerrainType == "WATER")
	{
		maxSpeed        = mv_WaterSpeed;
		airControl      = mv_WaterAirControl;
		acceleration    = mv_WaterAcceleration;
		jumpZVelocity   = mv_WaterJumpZVelocity;
		fallingFriction = mv_WaterFallingFriction;
	}
	else if (mv_TerrainType == "DEFAULT")
	{
		maxSpeed        = mv_DefaultSpeed;
		airControl      = mv_DefaultAirControl;
		acceleration    = mv_DefaultAcceleration;
		jumpZVelocity   = mv_DefaultJumpZVelocity;
		fallingFriction = mv_DefaultFallingFriction;
	}

	mv_MaxSpeed = maxSpeed;
	mc_character->GetCharacterMovement()->MaxWalkSpeed           = maxSpeed;
	mc_character->GetCharacterMovement()->AirControl             = airControl;
	mc_character->GetCharacterMovement()->JumpZVelocity          = jumpZVelocity;
	mc_character->GetCharacterMovement()->MaxAcceleration        = acceleration;
	mc_character->GetCharacterMovement()->FallingLateralFriction = fallingFriction;
}

void UTerrainManager::UpdateTerrainSettings(const RockMomemtumSettings& settings)
{
	mv_MomemtumActive   = settings.IsActive;
	mv_MomemtumSquare   = settings.IsSquare;
	mv_MomemtumMinValue = settings.MinValue;
}