/// \file       CameraSettings.h
/// \date       05/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_CAMERA_SETTINGS_H
#define ATTUNED_CAMERA_SETTINGS_H

#include "CoreMinimal.h"

struct CameraSettings
{
	CameraSettings();

	bool  bUsePawnControlRotation;
	float FieldOfView;
	float Pitch;
};

#endif // ATTUNED_CAMERA_SETTINGS_H