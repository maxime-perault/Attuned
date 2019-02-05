// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TerrainManager.h"
#include "CameraManager.h"
#include "W_InGameUI.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "MyCharacter.generated.h"

UCLASS(config = Game)
class ATTUNED_API AMyCharacter : public ACharacter
{
private:
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default, meta = (AllowPrivateAccess = "true"))
		class	UTextRenderComponent* mc_MoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default, meta = (AllowPrivateAccess = "true"))
		class	UTextRenderComponent* mc_JumpSpeed;

	void	Dash(const bool InitDash);

	float	mv_DebugFlushTime;
	float	mv_DeltaTime;
	float	mv_LeanPercent;
	float	mv_ForwardSpeed;
	bool	mv_LockControls;
	bool	mv_CanPlay;

public:
	AMyCharacter();

	// Override BeginPlay()
	virtual void BeginPlay() override;

	/** UI Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UW_InGameUI> mc_InGameUIC;

	UW_InGameUI*	mc_InGameUIAttached;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup, meta = (AllowPrivateAccess = "true"))
		UTerrainManager *mc_TerrainManager;

	UPROPERTY(VisibleAnywhere, Category = Setup, meta = (AllowPrivateAccess = "true"))
		UCameraManager *mc_CameraManager;

	/*
	** CAMERAS
	*/

		/*
		** STANDARD CAMERA
		*/

			/** Camera boom positioning the camera behind the character */
			UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
				class	USpringArmComponent* mc_DefaultCameraBoom;

			/** Follow camera */
			UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
				class	UCameraComponent* mc_DefaultFollowCamera;

			/** Camera Collision */
			UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
				class	USphereComponent* mc_DefaultCameraCollision;

		/*
		** WATER CAMERA
		*/

			/** Camera boom positioning the camera behind the character */
			UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
				class	USpringArmComponent* mc_WaterCameraBoom;

			/** Follow camera */
			UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
				class	UCameraComponent* mc_WaterFollowCamera;

			/** Camera Collision */
			UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
				class	USphereComponent* mc_WaterCameraCollision;

		/*
		** CURRENT CAM
		*/
			class	USpringArmComponent*	mc_CurrentCameraBoom;
			class	UCameraComponent*		mc_CurrentFollowCamera;
			class	USphereComponent*		mc_CurrentCameraCollision;

	/*
	** !END CAMERAS
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		float mv_DashPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		int mv_MenuPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		bool mv_DrawSpeedParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class URadialForceComponent* mc_DashRadialForce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class URadialForceComponent* mc_JumpRadialForce;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	UFUNCTION(BlueprintCallable, Category = "Get")
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return mc_CurrentCameraBoom; }
	/** Returns FollowCamera subobject **/
	UFUNCTION(BlueprintCallable, Category = "Get")
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return mc_CurrentFollowCamera; }

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Set")
		void UnlockControls();

	UFUNCTION(BlueprintCallable, Category = "Set")
		void lockControls();

	UFUNCTION(BlueprintCallable, Category = "Get")
		FString GetTerrainSurfaceType();

	UFUNCTION(BlueprintCallable, Category = "Get")
		bool GetStartPlay();
		

	UFUNCTION(BlueprintCallable, Category = "Get")
		bool GetCanPlay();

	UFUNCTION(BlueprintCallable, Category = "Get")
		float GetLeanDegree();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool	mv_isDashing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool	mv_Menu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool	mv_StartPlay;

	float	LerpForwardSpeed(const float NewSpeed, const float DeltaTime, const bool reset);

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