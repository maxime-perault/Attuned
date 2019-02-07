/// \file       CameraManager.cpp
/// \date       05/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Vincent STEHLY--CALISTO

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Curves/CurveFloat.h"
#include "Public/CollisionQueryParams.h"

#include "Camera/CameraProfile.h"
#include "CameraManager.generated.h"

class AMyCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATTUNED_API UCameraManager : public UActorComponent
{
public:

	GENERATED_BODY()

public:

	/// \brief Constructor, Sets default values for this component's properties
	UCameraManager();

	/// \brief Initializes all camera profiles. The previous camera profiles is set
	///		   to the new camera profile
	void Initialize	(void);

	/// \brief Called when the character walks on a new terrain
	/// \param type The type of the terrain (TODO : Change for an enum)
	void OnTerrainChange(int type);

	/// \brief  Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const CameraProfile& GetCurrentProfile  (void) const { return CurrentProfile;  }
	const CameraProfile& GetPreviousProfile (void) const { return PreviousProfile; }
	const CameraProfile& GetRockProfile		(void) const { return RockProfile;     }
	const CameraProfile& GetSandProfile		(void) const { return SandProfile;     }
	const CameraProfile& GetWaterProfile	(void) const { return WaterProfile;    }
	const CameraProfile& GetNeutralProfile	(void) const { return NeutralProfile;  }

	float GetMinPitch			(void)						const { return MinPitch;			}
	float GetMaxPitch			(void)						const { return MaxPitch;			}
	float GetCurrentPitch		(void)						const { return CurrentPitch;		}
	float GetMaxArmLenght		(void)						const { return MaxArmLength;		}
	float GetPercentBetweenAB	(float x, float a, float b) const { return ((x - a) / (b - a)); }

public:

	// TODO : Rename ?
	UPROPERTY(EditDefaultsOnly, Category = Curve)
	UCurveFloat* mc_ArmLengthCurveFromPitch;

protected:

	virtual void BeginPlay() override;

private:

	// TODO : Update code, Generic function ?
	void LerpArmLenght	();
	void LerpFieldOfView();

	/// \brief Compute the current pitch between -1 and 1
	void UpdatePitch					(void);
	void UpdateArmFromSpeed				(void);
	void UpdateCameraValues				(void);
	void UpdateCameraFromPitch			(void);
	void LerpCameraValues               (void);
	
	/// \brief Called after a camera boom to zoom out progressively instead of warping the camera
	void  ZoomOut							(void);
	void  CollisionBetweenCameraAndTarget	(void);
	float ComputeTrueArmLength				(float ArmLength) const;

private:

	CameraProfile CurrentProfile;
	CameraProfile PreviousProfile;
	CameraProfile RockProfile;
	CameraProfile SandProfile;
	CameraProfile WaterProfile;
	CameraProfile NeutralProfile;

	float MinPitch;
	float MaxPitch;
	float MaxArmLength;
	float CurrentPitch;

	AMyCharacter*			Character;
	FCollisionQueryParams	RV_TraceParams;
	FVector					NextCameraLocation;

	bool bDebug;
	bool bInitialized;
	bool bIsLerping;

	float LerpElapsedTime;
	float MaxArmLengthFromPitch;
	float mv_DeltaTime;
	float DeltaSpeedFOV = 0.f;
	float NextArmLength;
	
private:

	static		 bool  IsZoomingOut;
	static		 float LastArmLenght;
	static const float ZoomOutMaxDuration;
	static       float ZoomOutCurrentDuration;
};