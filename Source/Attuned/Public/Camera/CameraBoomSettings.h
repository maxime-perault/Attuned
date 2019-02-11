/// \file       CameraBoomSettings.h
/// \date       05/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_CAMERA_BOOM_SETTINGS_H
#define ATTUNED_CAMERA_BOOM_SETTINGS_H

#include "CoreMinimal.h"

struct CameraBoomSettings
{
	CameraBoomSettings();

	bool		bInheritRoll;
	bool		bInheritPitch;
	bool		bDoCollisionTest;
	bool		bUserPawnControlRotation;
	bool		bEnableCameraLag;
	bool		bEnableCameraRotationLag;

	float		TargetArmLenght;
	float		CameraLagSpeed;
	float		CameraRotationLagSpeed;

	FVector		RelativeLocation;
	FRotator	RelativeRotation;
};

#endif // ATTUNED_CAMERA_BOOM_SETTINGS_H