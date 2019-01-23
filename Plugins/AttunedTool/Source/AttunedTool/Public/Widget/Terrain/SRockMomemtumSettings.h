/// \file       SRockMomemtumSettings.h
/// \date       22/01/2019
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_S_ROCK_MOMEMTUM_SETTINGS_H_
#define ATTUNED_TOOL_S_ROCK_MOMEMTUM_SETTINGS_H_

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include <SSpinBox.h>
#include <SCheckBox.h>

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

#include "Widget/ICustomWidget.h"

/// \brief Momemtum settings for rock terrain
/// \class SRockMomemtumSettings
class SRockMomemtumSettings : public ICustomWidget
{
public:

	SLATE_BEGIN_ARGS(SRockMomemtumSettings) {}
	SLATE_END_ARGS  ()

public:

	void Construct(const FArguments& InArgs);

private:

	FSlateColor GetExpandableBodyColor  () const;
	FSlateColor GetExpandableBorderColor() const;

	void OnActiveCheckBoxCheckStateChanged	(ECheckBoxState state);
	void OnSquareCheckBoxCheckStateChanged	(ECheckBoxState state);
	void OnMinMomemtumValueChanged			(float value);

	ECheckBoxState	IsActiveCheckBoxChecked	() const;
	ECheckBoxState	IsSquareCheckBoxChecked	() const;
	float			MinMomemtumValue		() const;

	TSharedPtr<SCheckBox>		m_activeCheckBox;
	TSharedPtr<SCheckBox>		m_squareCheckBox;
	TSharedPtr<SSpinBox<float>> m_minMomemtum;
};

#endif // ATTUNED_TOOL_S_ROCK_MOMEMTUM_SETTINGS_H_