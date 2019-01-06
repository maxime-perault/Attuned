// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/CollisionQueryParams.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "W_InGameUI.h"
#include "TerrainManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATTUNED_API UTerrainManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTerrainManager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void	DashCoolDown(const bool reset);

	FString		mv_TerrainType;
	FVector		mv_TerrainNormal;

	UW_InGameUI*	mc_InGameUIAttached;

	float		mv_RockSpeed;
	float		mv_RockAcceleration;
	float		mv_RockJumpZVelocity;

	float		mv_SandSpeed;
	float		mv_SandAcceleration;
	float		mv_SandJumpZVelocity;

	float		mv_WaterSpeed;
	float		mv_WaterAcceleration;
	float		mv_WaterJumpZVelocity;

	float		mv_DefaultSpeed;
	float		mv_DefaultAcceleration;
	float		mv_DefaultJumpZVelocity;

	bool		mv_CanDash;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FCollisionQueryParams	mv_RV_TraceParams;
	EPhysicalSurface		mv_LastSurfaceType;
	
	ACharacter*	mc_character;
	
	void	RockTerrain(void);
	void	SandTerrain(void);
	void	WaterTerrain(void);
	void	StandardTerrain(void);

	void	CharacterMoveSpeedTransition(const bool InitTransition);

	bool	mv_IsTransitioningSpeed;
	float	mv_DeltaTime;
	float	mv_MaxSpeed;
};
