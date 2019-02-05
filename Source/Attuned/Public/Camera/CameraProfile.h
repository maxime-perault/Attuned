/// \file       CameraProfile.h
/// \date       05/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_CAMERA_PROFILE_H
#define ATTUNED_CAMERA_PROFILE_H

#include "CoreMinimal.h"

#include "Camera/CameraSettings.h"
#include "Camera/CameraBoomSettings.h"
#include "Camera/CameraCollisionSettings.h"

struct CameraProfile
{
	CameraProfile	();
	CameraProfile	(const CameraProfile& other);

	CameraSettings			Settings;
	CameraBoomSettings		BoomSettings;
	CameraCollisionSettings CollisionSettings;
};

#endif // ATTUNED_CAMERA_PROFILE_H