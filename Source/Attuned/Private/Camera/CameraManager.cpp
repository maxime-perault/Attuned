/// \file       CameraManager.h
/// \date       05/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Vincent STEHLY--CALISTO

#include "Camera/CameraManager.h"

#include "MyCharacter.h"
#include "Engine/GameEngine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

/* static */	   bool  UCameraManager::IsZoomingOut           = false;
/* static */	   float UCameraManager::LastArmLenght          = 500.0f;
/* static */ const float UCameraManager::ZoomOutMaxDuration     = 2.0f;
/* static */       float UCameraManager::ZoomOutCurrentDuration = 0.0f;

/// \brief Constructor, Sets the default values for the component's properties
UCameraManager::UCameraManager()
{
	bDebug							  = false;
	bInitialized					  = false;
	PrimaryComponentTick.bCanEverTick = true;
}

/// \brief Initializes all camera profiles. The previous camera profiles is set
///		   to the new camera profile
void UCameraManager::Initialize(void)
{
	// Initializing all camera profiles
	// Rock camera profile
	RockProfile.Settings.Pitch					      	 = 0.24f;
	RockProfile.Settings.bUsePawnControlRotation	     = false;
	RockProfile.BoomSettings.bInheritRoll			     = false;
	RockProfile.BoomSettings.bInheritPitch			     = true;
	RockProfile.BoomSettings.bDoCollisionTest		     = false;
	RockProfile.BoomSettings.bUserPawnControlRotation    = true;
	RockProfile.BoomSettings.bEnableCameraLag            = false;
	RockProfile.BoomSettings.bEnableCameraRotationLag    = false;
	RockProfile.BoomSettings.TargetArmLenght             = 500.0f;
	RockProfile.BoomSettings.CameraLagSpeed              = 0.0f;
	RockProfile.BoomSettings.CameraRotationLagSpeed      = 0.0f;
	RockProfile.BoomSettings.RelativeLocation            = FVector (0.0f, 0.0f, 60.0f);
	RockProfile.BoomSettings.RelativeRotation            = FRotator(0.0f, 0.0f,  0.0f);
	RockProfile.CollisionSettings.SphereRadius           = 40.0f;
													     
	// Sand camera profile							     
	SandProfile.Settings.Pitch							 = 0.24f;
	SandProfile.Settings.bUsePawnControlRotation         = false;
	SandProfile.BoomSettings.bInheritRoll                = false;
	SandProfile.BoomSettings.bInheritPitch               = true;
	SandProfile.BoomSettings.bDoCollisionTest            = false;
	SandProfile.BoomSettings.bUserPawnControlRotation    = true;
	SandProfile.BoomSettings.bEnableCameraLag		     = false;
	SandProfile.BoomSettings.bEnableCameraRotationLag    = false;
	SandProfile.BoomSettings.TargetArmLenght             = 500.0f;
	SandProfile.BoomSettings.CameraLagSpeed              = 0.0f;
	SandProfile.BoomSettings.CameraRotationLagSpeed      = 0.0f;
	SandProfile.BoomSettings.RelativeLocation            = FVector (0.0f, 0.0f, 60.0f);
	SandProfile.BoomSettings.RelativeRotation            = FRotator(0.0f, 0.0f,  0.0f);
	SandProfile.CollisionSettings.SphereRadius           = 40.0f;

	// Water camera profile
	WaterProfile.Settings.Pitch							 = 0.12f;
	WaterProfile.Settings.bUsePawnControlRotation        = false;
	WaterProfile.BoomSettings.bInheritRoll               = false;
	WaterProfile.BoomSettings.bInheritPitch              = true;
	WaterProfile.BoomSettings.bDoCollisionTest           = false;
	WaterProfile.BoomSettings.bUserPawnControlRotation   = true;
	WaterProfile.BoomSettings.bEnableCameraLag		     = false;
	WaterProfile.BoomSettings.bEnableCameraRotationLag   = true;
	WaterProfile.BoomSettings.TargetArmLenght            = 600.0f;
	WaterProfile.BoomSettings.CameraLagSpeed		     = 0.f;
	WaterProfile.BoomSettings.CameraRotationLagSpeed     = 7.0f;
	WaterProfile.BoomSettings.RelativeLocation           = FVector (7.0f,  0.0f, 60.0f);
	WaterProfile.BoomSettings.RelativeRotation           = FRotator(0.0f, 10.0f,  0.0f);
	WaterProfile.CollisionSettings.SphereRadius          = 40.0f;

	// Neutral camera profile
	NeutralProfile.Settings.Pitch						 = 0.24f;
	NeutralProfile.Settings.bUsePawnControlRotation      = false;
	NeutralProfile.BoomSettings.bInheritRoll             = false;
	NeutralProfile.BoomSettings.bInheritPitch            = true;
	NeutralProfile.BoomSettings.bDoCollisionTest         = false;
	NeutralProfile.BoomSettings.bUserPawnControlRotation = true;
	NeutralProfile.BoomSettings.bEnableCameraLag		 = false;
	NeutralProfile.BoomSettings.bEnableCameraRotationLag = false;
	NeutralProfile.BoomSettings.TargetArmLenght          = 500.0f;
	NeutralProfile.BoomSettings.CameraLagSpeed		     = 0.0f;
	NeutralProfile.BoomSettings.CameraRotationLagSpeed   = 0.0f;
	NeutralProfile.BoomSettings.RelativeLocation         = FVector (0.0f, 0.0f, 60.0f);
	NeutralProfile.BoomSettings.RelativeRotation         = FRotator(0.0f, 0.0f,  0.0f);
	NeutralProfile.CollisionSettings.SphereRadius        = 40.0f;

	CurrentProfile  = NeutralProfile;
	PreviousProfile = CurrentProfile;

	// Gets the owner object (The character)
	Character = StaticCast<AMyCharacter*>(GetOwner());

	// Initial settings
	Character->mc_CurrentFollowCamera->bUsePawnControlRotation = CurrentProfile.Settings.bUsePawnControlRotation;
	Character->mc_CurrentCameraBoom->bInheritRoll              = CurrentProfile.BoomSettings.bInheritRoll;
	Character->mc_CurrentCameraBoom->bInheritPitch             = CurrentProfile.BoomSettings.bInheritPitch;
	Character->mc_CurrentCameraBoom->bDoCollisionTest          = CurrentProfile.BoomSettings.bDoCollisionTest;
	Character->mc_CurrentCameraBoom->bUsePawnControlRotation   = CurrentProfile.BoomSettings.bUserPawnControlRotation;
	Character->mc_CurrentCameraBoom->bEnableCameraRotationLag  = CurrentProfile.BoomSettings.bEnableCameraRotationLag;
	Character->mc_CurrentCameraBoom->bEnableCameraLag		   = CurrentProfile.BoomSettings.bEnableCameraLag;
	Character->mc_CurrentCameraBoom->TargetArmLength		   = CurrentProfile.BoomSettings.TargetArmLenght;
	Character->mc_CurrentCameraBoom->CameraLagSpeed		       = CurrentProfile.BoomSettings.CameraLagSpeed;
	Character->mc_CurrentCameraBoom->CameraRotationLagSpeed    = CurrentProfile.BoomSettings.CameraRotationLagSpeed;
	Character->mc_CurrentCameraBoom->RelativeLocation		   = CurrentProfile.BoomSettings.RelativeLocation;
	Character->mc_CurrentCameraBoom->RelativeRotation	       = CurrentProfile.BoomSettings.RelativeRotation;

	// Collision radius
	Character->mc_CurrentCameraCollision->SetSphereRadius(CurrentProfile.CollisionSettings.SphereRadius);

	// Common variables
	MinPitch               = -0.5f;
	MaxPitch               =  0.9f;
	LerpElapsedTime        =  0.0f;
	bIsLerping             = false;
	ArmFromPitchMultiplier = 1.0f;

	MaxArmLength = Character->mc_CurrentCameraBoom->TargetArmLength;

	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera manager initialized."));
		UE_LOG(LogTemp, Warning, TEXT("Dont forget to set ArmLengthCurveFromPitch"));
	}

	bInitialized = true;
}

/* Override */ void UCameraManager::BeginPlay()
{
	Super::BeginPlay();
}

/// \brief Called when the character walks on a new terrain
/// \param type The type of the terrain (TODO : Change for an enum)
void UCameraManager::OnTerrainChange(int type)
{
	FVector FollowPosition  = Character->mc_CurrentFollowCamera->GetComponentLocation();
	FVector CapsulePosition = Character->GetCapsuleComponent()->GetComponentLocation();

	PreviousProfile = CurrentProfile;
	PreviousProfile.Settings.Pitch				  = CurrentPitch;
	PreviousProfile.BoomSettings.TargetArmLenght  = MaxArmLength;
	PreviousProfile.BoomSettings.RelativeLocation = Character->mc_CurrentCameraBoom->RelativeLocation;
	PreviousProfile.BoomSettings.RelativeRotation = Character->mc_CurrentCameraBoom->RelativeRotation;

	switch (type)
	{
		case 0: { CurrentProfile = NeutralProfile; break; }
		case 1:	{ CurrentProfile = RockProfile;    break; }
		case 2: { CurrentProfile = WaterProfile;   break; }
		case 3: { CurrentProfile = NeutralProfile; break; }
		
		default:
		{
			CurrentProfile = NeutralProfile;
			break;
		}
	}

	bIsLerping      = true;
	LerpElapsedTime = 0.0;

	// Called to set values which doesn't need any lerp
	UpdateCameraValues();
}

void UCameraManager::UpdateCameraValues()
{
	Character->mc_CurrentFollowCamera->bUsePawnControlRotation = CurrentProfile.Settings.bUsePawnControlRotation;
	Character->mc_CurrentCameraBoom->bInheritRoll			   = CurrentProfile.BoomSettings.bInheritRoll;
	Character->mc_CurrentCameraBoom->bDoCollisionTest		   = CurrentProfile.BoomSettings.bDoCollisionTest;
	Character->mc_CurrentCameraBoom->bUsePawnControlRotation   = CurrentProfile.BoomSettings.bUserPawnControlRotation;
	Character->mc_CurrentCameraBoom->bEnableCameraLag		   = CurrentProfile.BoomSettings.bEnableCameraLag;
	Character->mc_CurrentCameraBoom->bEnableCameraRotationLag  = CurrentProfile.BoomSettings.bEnableCameraRotationLag;
	Character->mc_CurrentCameraBoom->CameraLagSpeed		       = CurrentProfile.BoomSettings.CameraLagSpeed;
	Character->mc_CurrentCameraBoom->CameraRotationLagSpeed    = CurrentProfile.BoomSettings.CameraRotationLagSpeed;

	// Collision radius
	Character->mc_CurrentCameraCollision->SetSphereRadius(CurrentProfile.CollisionSettings.SphereRadius);
}

void UCameraManager::LerpCameraValues(void)
{
	if (!bIsLerping)
	{
		return;
	}

	LerpElapsedTime += mv_DeltaTime;
	LerpElapsedTime = FMath::Clamp(LerpElapsedTime, 0.0f, 1.0f);

	Character->mc_CurrentCameraBoom->RelativeLocation = FMath::InterpEaseInOut(PreviousProfile.BoomSettings.RelativeLocation, CurrentProfile.BoomSettings.RelativeLocation, LerpElapsedTime, 2.5f);
	Character->mc_CurrentCameraBoom->RelativeRotation = FMath::InterpEaseInOut(PreviousProfile.BoomSettings.RelativeRotation, CurrentProfile.BoomSettings.RelativeRotation, LerpElapsedTime, 2.5f);

	if (Character->GetTerrainSurfaceType() != "WATER")
	{
		MaxArmLength = FMath::InterpEaseInOut(PreviousProfile.BoomSettings.TargetArmLenght, CurrentProfile.BoomSettings.TargetArmLenght, LerpElapsedTime, 2.5f);
	}

	// Compute the amount of pitch to add or remove
	float delta             = CurrentProfile.Settings.Pitch - PreviousProfile.Settings.Pitch;
	float MissingInputPitch = delta / 0.043620f;
	
	// Sending the input
	CastChecked<APlayerController>(Character->Controller)->AddPitchInput(MissingInputPitch * mv_DeltaTime);

	// LogTemp : Warning : Current Pitch : 0.000000
	// LogTemp : Warning : Current Pitch : 0.043620 = 1.0f

	if (LerpElapsedTime >= 1.0f)
	{
		bIsLerping                                     = false;
		LastTerrainVelocity							   = Character->GetVelocity().Size();
	    Character->mc_CurrentCameraBoom->bInheritPitch = CurrentProfile.BoomSettings.bInheritPitch;
	}
}

/// \brief  Called every frame
void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	mv_DeltaTime = DeltaTime;

	if (!bInitialized)
	{
		return;
	}

	// TODO : Use enum instead of string variables
	if (Character->GetTerrainSurfaceType() != "WATER")
	{
		Character->mv_DrawSpeedParticles = false;

		UpdateCameraFromPitch();
		CollisionBetweenCameraAndTarget();
		ZoomOut();
		
		Character->mc_CurrentFollowCamera->PostProcessSettings.SceneFringeIntensity = 0.f;
	}
	else
	{
		UpdateCameraFromPitch();
		CollisionBetweenCameraAndTarget();
		ZoomOut();
		UpdateArmFromSpeed();
	}

	LerpCameraValues();
	UpdatePitch();
}

void UCameraManager::LerpArmLenght()
{
	// TODO : Maybe reorder the methdod in the file
}

void UCameraManager::LerpFieldOfView()
{
	// TODO
}

/// \brief Compute the current pitch between -1 and 1
void UCameraManager::UpdatePitch(void)
{ 
	FVector TargetPos;
	FVector FakeCameraArm;
	
	TargetPos     = Character->mc_CurrentCameraBoom->GetComponentLocation();
	FakeCameraArm = Character->mc_CurrentFollowCamera->GetForwardVector();
	
	FakeCameraArm.Normalize();
	FakeCameraArm *= MaxArmLength;
	
	CurrentPitch = ((TargetPos - FakeCameraArm).Z - Character->mc_CurrentCameraBoom->GetComponentLocation().Z) / MaxArmLength;
}

void UCameraManager::UpdateArmFromSpeed(void)
{
	// TODO : Refactor terrain manager
	float percent = FMath::Clamp(GetPercentBetweenAB(Character->GetVelocity().Size(), LastTerrainVelocity, Character->mc_TerrainManager->mv_WaterSpeed), 0.0f, 1.0f);
	
	if (bIsLerping)
	{
		MaxArmLength  = FMath::InterpEaseInOut(PreviousProfile.BoomSettings.TargetArmLenght, CurrentProfile.BoomSettings.TargetArmLenght, LerpElapsedTime, 2.5f);
	}
	else
	{
		MaxArmLength = FMath::Lerp(CurrentProfile.BoomSettings.TargetArmLenght, CurrentProfile.BoomSettings.TargetArmLenght + 150.0f, percent);
	}

	if (percent >= 0.7f)
	{
		const float SpeedPercent = GetPercentBetweenAB(
			Character->GetVelocity().Size(),
			Character->mc_TerrainManager->mv_WaterSpeed * 0.7f,
			Character->mc_TerrainManager->mv_WaterSpeed * 0.8f);

		// Chromatic aberration
		Character->mc_CurrentFollowCamera->PostProcessSettings.SceneFringeIntensity = SpeedPercent * 4.0f;
		DeltaSpeedFOV = SpeedPercent * 8.0f;

	}
	else
	{
		DeltaSpeedFOV = 0.0f;
	}

	// If the speed is high enough, we enable particles for the sense of speed
	Character->mv_DrawSpeedParticles = (percent >= 0.8f);
}

void UCameraManager::UpdateCameraFromPitch(void)
{
	UpdatePitch();

	// TODO : Move the following variables into the appropriate profiles
	static const float BaseFOV (90.0f);
	static const float DeltaFOV(10.0f);
	static const float BaseCameraBoomZ (60.0f);
	static const float DeltaCameraBoomZ(40.0f);

	// The curve is between 0 and 2 and the pitch is between -1 and 1
	MaxArmLengthFromPitch = mc_ArmLengthCurveFromPitch->GetFloatValue(CurrentPitch + 1) * MaxArmLength;
	
	Character->mc_CurrentCameraBoom->SetRelativeLocation(
		FVector(0.0f, 0.0f,
			BaseCameraBoomZ + FMath::Lerp(
				DeltaCameraBoomZ,
				0.0f,
				this->GetPercentBetweenAB(CurrentPitch, MinPitch, MaxPitch))));

	// Updates the FOV when the camera is near of the character
	Character->mc_CurrentFollowCamera->SetFieldOfView(
		BaseFOV + FMath::Lerp(
			DeltaFOV,
			0.f,
			this->GetPercentBetweenAB(CurrentPitch, MinPitch, MaxPitch)));

	NextCameraLocation  = Character->mc_CurrentFollowCamera->GetForwardVector();
	NextCameraLocation.Normalize();
	NextCameraLocation *= MaxArmLengthFromPitch;
	NextCameraLocation  = Character->mc_CurrentCameraBoom->GetComponentLocation() - NextCameraLocation;
}

/// \brief Called after a camera boom to zoom out progressively instead of warping the camera
void UCameraManager::ZoomOut(void)
{
	if ((LastArmLenght > NextArmLength) && 
		(NextArmLength < MaxArmLengthFromPitch))
	{
		IsZoomingOut           = true;
		ZoomOutCurrentDuration = (NextArmLength / MaxArmLengthFromPitch) * ZoomOutMaxDuration;
		Character->mc_CurrentCameraBoom->TargetArmLength = ComputeTrueArmLength(NextArmLength);

		LastArmLenght = NextArmLength;
		if (GEngine && bDebug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, 
				FString("INIT ZOOM Out : ") + 
				FString::SanitizeFloat(NextArmLength) + FString("; ") + 
				FString::SanitizeFloat(MaxArmLengthFromPitch));
		}
			
		return;
	}

	if (IsZoomingOut && (LastArmLenght != NextArmLength))
	{
		ZoomOutCurrentDuration += mv_DeltaTime;
		if (ZoomOutCurrentDuration > ZoomOutMaxDuration)
		{
			ZoomOutCurrentDuration = ZoomOutMaxDuration;
		}
			
		const float	ZoomingOutArmLength(FMath::Lerp(0.f, 1.f, ZoomOutCurrentDuration / ZoomOutMaxDuration) * MaxArmLengthFromPitch);

		Character->mc_CurrentCameraBoom->TargetArmLength = ComputeTrueArmLength(ZoomingOutArmLength);
		LastArmLenght = ZoomingOutArmLength;

		if (ZoomOutCurrentDuration == ZoomOutMaxDuration)
		{
			if (GEngine && bDebug)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, 
					FString("END Zooming Out"));
			}
				
			IsZoomingOut = false;
		}

		if (GEngine && bDebug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, 
				FString("Zooming Out: ") +
				FString::SanitizeFloat(ZoomOutCurrentDuration / ZoomOutMaxDuration));
		}	
	}
	else
	{
		Character->mc_CurrentCameraBoom->TargetArmLength = ComputeTrueArmLength(NextArmLength);
		LastArmLenght = NextArmLength;
	}
}

float UCameraManager::ComputeTrueArmLength(float ArmLength) const
{
	return (ArmLength - Character->mc_CurrentCameraCollision->GetScaledSphereRadius());
}

void UCameraManager::CollisionBetweenCameraAndTarget(void)
{
	FHitResult				RV_Hit(ForceInit);
	static bool				success;
	static TArray<AActor*>	FoundActors;

	//Raycast from Target to Camera
	success = GetWorld()->LineTraceSingleByChannel(
		RV_Hit,														// result
		Character->mc_CurrentCameraBoom->GetComponentLocation(),	// start
		NextCameraLocation,											// end
		ECC_Camera,													// collision channel
		RV_TraceParams												// params
	);

	if (!success)
	{
		Character->mc_CurrentCameraCollision->SetWorldLocation(NextCameraLocation);
		Character->mc_CurrentCameraCollision->GetOverlappingActors(FoundActors, nullptr);

		if (FoundActors.Num() > 0)
		{
			success = GetWorld()->LineTraceSingleByChannel(
				RV_Hit,														// result
				Character->mc_CurrentCameraBoom->GetComponentLocation(),	// start
				NextCameraLocation,											// end
				ECC_Visibility,												// collision channel
				RV_TraceParams												// params
			);

			if (!success)
			{
				NextArmLength = MaxArmLengthFromPitch;
				return;
			}

			NextArmLength = RV_Hit.Distance;
		}
		else
		{
			NextArmLength = MaxArmLengthFromPitch;
		}

		return;
	}

	if ((RV_Hit.Distance > 0.f) &&
		(RV_Hit.Distance < MaxArmLengthFromPitch))
	{
		NextArmLength = RV_Hit.Distance;
	}
}