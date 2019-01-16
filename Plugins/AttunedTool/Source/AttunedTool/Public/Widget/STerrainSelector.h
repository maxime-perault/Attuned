/// \file       STerrainSelector.h
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_S_TERRAIN_SELECTOR_H_
#define ATTUNED_TOOL_S_TERRAIN_SELECTOR_H_

#include "SSpinBox.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

#include "Widget/ICustomWidget.h"

/// \brief Allows the user to pick the terrain to modify
/// \class STerrainSelector
class STerrainSelector : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(STerrainSelector){}
	SLATE_END_ARGS  ()

public:

	void Construct(const FArguments& InArgs);

	/// \brief Updates the terrain tab index
	FReply OnRockTerrainClick    ();
	FReply OnSandTerrainClick    ();
	FReply OnWaterTerrainClick   ();
	FReply OnNeutralTerrainClick ();

private:

	/// \brief Returns the current tab index
	///  0 - Rock    terrain tab
	///  1 - Sand    terrain tab
	///  2 - Water   terrain tab
	///  3 - Neutral terrain tab
	/// \return The terrain tab index
	int32        GetCurrentTabIndex() const;

	/// \brief Returns the color associated to a terrain
	FLinearColor GetTerrainColor          ()				const;
	FLinearColor GetButtonColor	          (int32 index)		const;
	FLinearColor GetButtonBackgroundColor ()				const;
	FLinearColor GetButtonForegroundColor ()				const;

	FReply ApplyChanges ();
	FReply RevertChanges();

	void OnCameraMaxArmLenghtChange        (float value);
	void OnCameraMaxTimeFromLastInputChange(float value);

	float CameraMaxArmLenghtValue        () const;
	float CameraMaxTimeFromLastInputValue() const;

private:

	int32								m_tabIndex;
	TArray<TSharedPtr<ICustomWidget>>	m_widgets;

	TSharedPtr<SSpinBox<float>>			m_cameraMaxArmLenght;
	TSharedPtr<SSpinBox<float>>			m_cameraMaxTimeFromLastInput;
};

#endif // ATTUNED_TOOL_S_TERRAIN_SELECTOR_H_