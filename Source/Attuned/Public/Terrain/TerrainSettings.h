/// \file       TerrainSettings.h
/// \date       09/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TERRAIN_SETTINGS_H
#define ATTUNED_TERRAIN_SETTINGS_H

/// \brief Struct containing terrain settings
struct ATTUNED_API TerrainSettings
{
	float Speed;
	float AirControl;
	float Acceleration;
	float JumpZVelocity;
	float FallingFriction;
};

/// \brief Struct containing the settings of the rock momemtum
struct ATTUNED_API RockMomemtumSettings
{
	bool  IsActive;
	bool  IsSquare;
	float MinValue;
};

#endif // ATTUNED_TERRAIN_SETTINGS_H