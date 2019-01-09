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

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATTUNED_API UCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCameraManager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Initialize(USpringArmComponent* CameraBoom, UCameraComponent* FollowCamera, USphereComponent* CameraCollision);

	UPROPERTY(EditDefaultsOnly, Category = Curve)
		UCurveFloat* ArmLengthCurveFromPitch;

	float	mv_CurrentPitch;
	float	mv_MinPitch;
	float	mv_MaxPitch;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	USpringArmComponent*	mc_CameraBoom;
	UCameraComponent*		mc_FollowCamera;
	USphereComponent*		mc_CameraCollision;

	FCollisionQueryParams	mv_RV_TraceParams;

	bool	mv_initialized;
	bool	mv_debug;

	float	mv_MaxArmLength;
	float	mv_MaxArmLengthFromPitch;

	float	mv_DeltaTime;

	float	mv_NextArmLength;
	FVector	mv_NextCameraLocation;

	void	UpdateCameraFromPitch(void);
	void	UpdatePitch(void);
	float	getPercentBetweenAB(float x, float a, float b);
	void	CollisionBetweenCameraAndTarget(void);
	void	ZoomOut(void);

	FORCEINLINE float GetArmLength(float ArmLength) const { return (ArmLength - mc_CameraCollision->GetScaledSphereRadius()); }
};
