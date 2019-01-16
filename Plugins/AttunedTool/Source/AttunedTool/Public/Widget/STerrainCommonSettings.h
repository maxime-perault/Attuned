/// \file       STerrainCommonSettings.h
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_S_TERRAIN_COMMON_SETTINGS_H_
#define ATTUNED_TOOL_S_TERRAIN_COMMON_SETTINGS_H_

#include "SSpinBox.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

#include "Global/GAttunedTool.h"
#include "Widget/ICustomWidget.h"

/// \brief Common settings for all terrains
/// \class STerrainCommonSettings
class STerrainCommonSettings : public ICustomWidget
{
public:

	enum ETerrainType
	{
		Rock,
		Sand,
		Water,
		Neutral
	};

public:

	SLATE_BEGIN_ARGS(STerrainCommonSettings) {}
		SLATE_ARGUMENT(ETerrainType, terrainType)
		SLATE_ARGUMENT(TSharedPtr<SSpinBox<float>>, cameraMaxArmLenght)
		SLATE_ARGUMENT(TSharedPtr<SSpinBox<float>>, cameraMaxTimeFromLastInput)
	SLATE_END_ARGS  ()

public:

	void Construct(const FArguments& InArgs);
	void Tick     (const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

	void ApplyChanges() final;
	void ResetChanges() final;

private:

	FSlateColor GetExpandableBodyColor  () const;
	FSlateColor GetExpandableBorderColor() const;

	void OnFallingFrictionValueChanged	(float value);
	void OnAirControlValueChanged		(float value);
	void OnJumpZVelocityValueChanged	(float value);
	void OnDashCooldownValueChanged		(float value);
	void OnAccelerationValueChanged		(float value);
	void OnMaxSpeedValueChanged			(float value);

	float FallingFrictionValue	() const;
	float AirControlValue		() const;
	float JumpZVelocityValue	() const;
	float DashCooldownValue		() const;
	float AccelerationValue		() const;
	float MaxSpeedValue			() const;
	
	// Helper function to avoid duplicating code
	const CommonData*	GetCommonDataCache			()						        const;
	void				BuildCommonDataFromWidget	(CommonDataGeneric& data)       const;
	void				UpdateModelFromCommonData   (const CommonDataGeneric& data) const;

private:

	ETerrainType				m_terrainType;

	TSharedPtr<SSpinBox<float>> m_cameraMaxArmLenght;
	TSharedPtr<SSpinBox<float>> m_cameraMaxTimeFromLastInput;

	TSharedPtr<SSpinBox<float>> m_fallingFriction;
	TSharedPtr<SSpinBox<float>> m_airControl;
	TSharedPtr<SSpinBox<float>> m_jumpZVelocity;
	TSharedPtr<SSpinBox<float>> m_dashCooldown;
	TSharedPtr<SSpinBox<float>> m_acceleration;
	TSharedPtr<SSpinBox<float>> m_maxSpeed;
};

#endif // ATTUNED_TOOL_S_TERRAIN_COMMON_SETTINGS_H_