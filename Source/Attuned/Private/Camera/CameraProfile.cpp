/// \file       CameraProfile.cpp
/// \date       05/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Vincent STEHLY--CALISTO

#include "Camera/CameraProfile.h"

CameraProfile::CameraProfile()
{
	// None
}

CameraProfile::CameraProfile(const CameraProfile& other)
{
	Settings			= other.Settings;
	BoomSettings		= other.BoomSettings;
	CollisionSettings	= other.CollisionSettings;
}