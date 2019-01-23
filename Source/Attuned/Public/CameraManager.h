// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Curves/CurveFloat.h"
#include "Public/CollisionQueryParams.h"
#include "CameraManager.generated.h"

class AMyCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATTUNED_API UCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCameraManager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void	Initialize(void);
	void	SetOwner(AMyCharacter* owner);

	UPROPERTY(EditDefaultsOnly, Category = Curve)
		UCurveFloat* mc_ArmLengthCurveFromPitch;

	float	mv_CurrentPitch;
	float	mv_MinPitch;
	float	mv_MaxPitch;

	struct CameraSettings
	{
		float MaxArmLength;
		float MaxTimeFromLastInput;
	};

	void UpdateCameraSettings(const CameraSettings& settings);
	void OnTerrainChange(int type);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void LerpArmLenght	();
	void LerpFieldOfView();


	const float WaterArmLenght   = 600.0f;
	const float NeutralArmLenght = 600.0f;
	const float SandArmLenght    = 500.0f;
	const float RockArmLenght    = 350.0f;

	float previousArmLenght;
	float currentArmLenght;

private:
	AMyCharacter*			mc_character;

	FCollisionQueryParams	mv_RV_TraceParams;

	bool	mv_initialized;
	bool	mv_debug;

	float	mv_MaxArmLength;
	float   mv_MaxTimeFromLastInput;
	float	mv_MaxArmLengthFromPitch;
	
	float	mv_DeltaTime;

	float	mv_NextArmLength;
	FVector	mv_NextCameraLocation;

	void	UpdateCameraFromPitch(void);
	void	UpdatePitch(void);
	float	GetPercentBetweenAB(float x, float a, float b);
	void	CollisionBetweenCameraAndTarget(void);
	void	ZoomOut(void);
	void	UpdateArmFromSpeed(void);

	float GetArmLength(float ArmLength) const;
};
