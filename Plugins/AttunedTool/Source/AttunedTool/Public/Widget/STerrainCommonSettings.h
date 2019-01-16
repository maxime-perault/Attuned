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

#include "Widget/ICustomWidget.h"

/// \brief Common settings for all terrains
/// \class STerrainCommonSettings
class STerrainCommonSettings : public ICustomWidget
{
public:

	SLATE_BEGIN_ARGS(STerrainCommonSettings) {}
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
	
private:

	// Camera (Default values)
	float m_maxArmLenghtValue         = 250.0f;
	float m_maxTimeFromLastInputValue = 2.0f;

	// Constants (Limits)
	const float m_maxArmLenghtMinValue         = 1.0f;
	const float m_maxArmLenghtMaxValue         = 1000.0f;
	const float m_maxTimeFromLastInputMinValue = 0.00f;
	const float m_maxTimeFromLastInputMaxValue = 20.0f;

	// Movements (Default values)
	float m_fallingFrictionValue = 50.0f;
	float m_airControlValue      = 50.0f;
	float m_jumpZVelocityValue   = 600.0f;
	float m_dashCooldownValue	 = 1.0f;
	float m_accelerationValue	 = 10.0f;
	float m_maxSpeedValue		 = 800.0f;

	// Constants (Limits)
	const float m_fallingFrictionMinValue = 0.0f;
	const float m_fallingFrictionMaxValue = 500.0f;
	const float m_airControlMinValue      = 0.0f;
	const float m_airControlMaxValue      = 500.0f;
	const float m_jumpZVelocityMinValue   = 0.0f;
	const float m_jumpZVelocityMaxValue   = 5000.0f;
	const float m_dashCooldownMinValue    = 0.0f;
	const float m_dashCooldownMaxValue    = 20.0f;
	const float m_accelerationMinValue    = 0.0f;
	const float m_accelerationMaxValue    = 5000.0f;
	const float m_maxSpeedMinValue        = 0.0f;
	const float m_maxSpeedMaxValue        = 10000.0f;

	//TMap<FString, TSharedRef<SSpinBox<float>>> m_values;
	TSharedPtr<SSpinBox<float>> m_cameraMaxArmLenght;
	TSharedPtr<SSpinBox<float>> m_cameraMaxTimeFromLastInput;
};

#endif // ATTUNED_TOOL_S_TERRAIN_COMMON_SETTINGS_H_