// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/CollisionQueryParams.h"
#include "GameFramework/Character.h"
//#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "W_InGameUI.h"
#include "TerrainManager.generated.h"

class AMyCharacter;

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
	void	SetOwner(AMyCharacter* owner);

	FString		mv_TerrainType;
	FVector		mv_TerrainNormal;

	UW_InGameUI*	mc_InGameUIAttached;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Terrain, meta = (AllowPrivateAccess = "true"))
	bool bOnWaterEnter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Terrain, meta = (AllowPrivateAccess = "true"))
	bool bOnWaterExit;

	struct RockTerrainSettings
	{
		float RockSpeed;
		float RockAirControl;
		float RockAcceleration;
		float RockJumpZVelocity;
		float RockFallingFriction;
	};

	struct SandTerrainSettings
	{
		float SandSpeed;
		float SandAirControl;
		float SandAcceleration;
		float SandJumpZVelocity;
		float SandFallingFriction;
	};

	struct WaterTerrainSettings
	{
		float WaterSpeed;
		float WaterAirControl;
		float WaterAcceleration;
		float WaterJumpZVelocity;
		float WaterFallingFriction;
	};

	struct NeutralTerrainSettings
	{
		float DefaultSpeed;
		float DefaultAirControl;
		float DefaultAcceleration;
		float DefaultJumpZVelocity;
		float DefaultFallingFriction;
	};

	struct RockMomemtumSettings
	{
		bool  IsActive;
		bool  IsSquare;
		float MinValue;
	};

	float	mv_RockSpeed;
	float	mv_RockAirControl;
	float	mv_RockAcceleration;
	float	mv_RockJumpZVelocity;
	float	mv_RockFallingFriction;

	float	mv_SandSpeed;
	float	mv_SandAirControl;
	float	mv_SandAcceleration;
	float	mv_SandJumpZVelocity;
	float	mv_SandFallingFriction;

	float	mv_WaterSpeed;
	float	mv_WaterAirControl;
	float	mv_WaterAcceleration;
	float	mv_WaterJumpZVelocity;
	float	mv_WaterFallingFriction;

	float	mv_DefaultSpeed;
	float	mv_DefaultAirControl;
	float	mv_DefaultAcceleration;
	float	mv_DefaultJumpZVelocity;
	float	mv_DefaultFallingFriction;

	bool	mv_MomemtumActive;
	bool	mv_MomemtumSquare;
	float   mv_MomemtumMinValue;

	bool	mv_CanDash;

public:

	void UpdateTerrainSettings(const RockTerrainSettings&    settings);
	void UpdateTerrainSettings(const SandTerrainSettings&    settings);
	void UpdateTerrainSettings(const WaterTerrainSettings&   settings);
	void UpdateTerrainSettings(const NeutralTerrainSettings& settings);
	void UpdateTerrainSettings(const RockMomemtumSettings&   settings);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void UpdateCharacterSettings();

	FCollisionQueryParams	mv_RV_TraceParams;
	EPhysicalSurface		mv_LastSurfaceType;
	
	AMyCharacter*	mc_character;
	
	void	RockTerrainFirstStep(void);
	void	SandTerrainFirstStep(void);
	void	WaterTerrainFirstStep(void);
	void	StandardTerrainFirstStep(void);

	void	CharacterMoveSpeedTransition(const bool InitTransition);

	bool	mv_initialized;
	bool	mv_IsTransitioningSpeed;
	float	mv_DeltaTime;
	float	mv_MaxSpeed;
};
