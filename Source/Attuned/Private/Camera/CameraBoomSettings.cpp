/// \file       CameraBoomSettings.cpp
/// \date       05/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Vincent STEHLY--CALISTO

#include "Camera/CameraBoomSettings.h"

CameraBoomSettings::CameraBoomSettings()
: bInheritRoll				(true)
, bInheritPitch				(true)
, bDoCollisionTest			(false)
, bUserPawnControlRotation	(true)
, bEnableCameraLag			(true)
, bEnableCameraRotationLag	(true)
, TargetArmLenght			(500.0f)
, CameraLagSpeed			(0.0f)
, CameraRotationLagSpeed	(0.0f)
, RelativeLocation			(0.0f, 0.0f, 60.0f)
, RelativeRotation			(0.0f, 0.0f,  0.0f)
{
	// None
}