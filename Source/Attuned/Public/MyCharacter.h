// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TerrainManager.h"
#include "CameraManager.h"
#include "W_InGameUI.h"
#include "DestructibleActor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "MyCharacter.generated.h"

UCLASS(config = Game)
class AMyCharacter : public ACharacter
{
private:
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Default, meta = (AllowPrivateAccess = "true"))
		class	UTextRenderComponent* mc_MoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = Default, meta = (AllowPrivateAccess = "true"))
		class	UTextRenderComponent* mc_JumpSpeed;

	UPROPERTY(VisibleAnywhere, Category = Setup, meta = (AllowPrivateAccess = "true"))
		UTerrainManager *mc_TerrainManager;

	UPROPERTY(VisibleAnywhere, Category = Setup, meta = (AllowPrivateAccess = "true"))
		UCameraManager *mc_CameraManager;

	void	Dash(const bool InitDash);

	float	mv_DebugFlushTime;
	float	mv_DeltaTime;
	bool	mv_LockControls;

public:
	AMyCharacter();

	// Override BeginPlay()
	virtual void BeginPlay() override;

	/** UI Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UW_InGameUI> mc_InGameUIC;

	UW_InGameUI*	mc_InGameUIAttached;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class	UCameraComponent* FollowCamera;

	/** Camera Collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class	USphereComponent* CameraCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class URadialForceComponent* mc_DashRadialForce;


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Get")
		FString getTerrainSurfaceType();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool	mv_isDashing;

protected:

	virtual void Jump() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void	UpdateDebugTextLocation(void);
};