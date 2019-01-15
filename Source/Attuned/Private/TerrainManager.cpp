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
	mv_WaterAcceleration = 2048.f;
	mv_WaterSpeed = 1000.f;

	mv_SandJumpZVelocity = 1000.f;
	mv_SandAcceleration = 1000.f;
	mv_SandSpeed = 1000.f;
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

	if ((mv_LastSurfaceType != CurrentSurface)
		&& CurrentSurface != SurfaceType4) // DestructibleMeshSurface
	{
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
	mc_character->GetCharacterMovement()->JumpZVelocity = mv_DefaultJumpZVelocity;
	mc_character->GetCharacterMovement()->MaxAcceleration = mv_DefaultAcceleration;
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
	mc_character->GetCharacterMovement()->JumpZVelocity = mv_RockJumpZVelocity; //Not Used
	mc_character->GetCharacterMovement()->MaxAcceleration = mv_RockAcceleration;
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
	mc_character->GetCharacterMovement()->JumpZVelocity = mv_WaterJumpZVelocity;
	mc_character->GetCharacterMovement()->MaxAcceleration = mv_WaterAcceleration;
	mv_MaxSpeed = mv_WaterSpeed;

	mc_character->mc_DefaultFollowCamera->Deactivate();
	mc_character->mc_WaterFollowCamera->Activate();

	mc_character->mc_CurrentCameraBoom = mc_character->mc_WaterCameraBoom;
	mc_character->mc_CurrentFollowCamera = mc_character->mc_WaterFollowCamera;
	mc_character->mc_CurrentCameraCollision = mc_character->mc_WaterCameraCollision;

	this->CharacterMoveSpeedTransition(true);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Water"));
}

void UTerrainManager::SandTerrainFirstStep(void)
{
	if (mc_InGameUIAttached)
		mc_InGameUIAttached->SetVisibility(ESlateVisibility::Hidden); // If Rock before

	mv_TerrainType = TEXT("SAND");
	mc_character->GetCharacterMovement()->JumpZVelocity = mv_SandJumpZVelocity;
	mc_character->GetCharacterMovement()->MaxAcceleration = mv_SandAcceleration;
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

