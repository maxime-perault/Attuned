/// \file       STerrainSelector.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/STerrainSelector.h"
#include "Widget/Terrain/SRockTerrainSettings.h"
#include "Widget/Terrain/SSandTerrainSettings.h"
#include "Widget/Terrain/SWaterTerrainSettings.h"
#include "Widget/Terrain/SNeutralTerrainSettings.h"

#include <SBox.h>
#include <SImage.h>
#include <SButton.h>
#include <SWrapBox.h>
#include <STextBlock.h>
#include <SWidgetSwitcher.h>

#include <Framework/MultiBox/MultiBox.h>
#include <Framework/MultiBox/MultiBoxDefs.h>
#include <Framework/MultiBox/MultiBoxBuilder.h>

#include "AttunedToolStyle.h"
#include "Model/AttunedModel.h"
#include "Global/GAttunedTool.h"

#define LOCTEXT_NAMESPACE "STerrainSelector"

void STerrainSelector::Construct(const FArguments& InArgs)
{
	m_tabIndex = 0;

	m_cameraMaxArmLenght = SNew(SSpinBox<float>)
		.Value         (CameraData::MaxArmLenghtDefaultValue)
		.MinValue      (CameraData::MaxArmLenghtMinValue)
		.MaxValue      (CameraData::MaxArmLenghtMaxValue)
		.OnValueChanged(this, &STerrainSelector::OnCameraMaxArmLenghtChange);

	m_cameraMaxTimeFromLastInput = SNew(SSpinBox<float>)
		.Value         (CameraData::MaxTimeFromLastInputDefaultValue)
		.MinValue      (CameraData::MaxTimeFromLastInputMinValue)
		.MaxValue      (CameraData::MaxTimeFromLastInputMaxValue)
		.OnValueChanged(this, &STerrainSelector::OnCameraMaxTimeFromLastInputChange);

	TSharedPtr<SRockTerrainSettings> rockTerrainWidget = SNew(SRockTerrainSettings)
		.cameraMaxArmLenght		   (m_cameraMaxArmLenght)
		.cameraMaxTimeFromLastInput(m_cameraMaxTimeFromLastInput);

	TSharedPtr<SSandTerrainSettings> sandTerrainWidget = SNew(SSandTerrainSettings)
		.cameraMaxArmLenght        (m_cameraMaxArmLenght)
		.cameraMaxTimeFromLastInput(m_cameraMaxTimeFromLastInput);

	TSharedPtr<SWaterTerrainSettings> waterTerrainWidget = SNew(SWaterTerrainSettings)
		.cameraMaxArmLenght        (m_cameraMaxArmLenght)
		.cameraMaxTimeFromLastInput(m_cameraMaxTimeFromLastInput);

	TSharedPtr<SNeutralTerrainSettings> neutralTerrainWidget = SNew(SNeutralTerrainSettings)
		.cameraMaxArmLenght        (m_cameraMaxArmLenght)
		.cameraMaxTimeFromLastInput(m_cameraMaxTimeFromLastInput);

	m_widgets.Empty();
	m_widgets.Add(rockTerrainWidget);
	m_widgets.Add(sandTerrainWidget);
	m_widgets.Add(waterTerrainWidget);
	m_widgets.Add(neutralTerrainWidget);

	ChildSlot
	[
		SNew(SBorder)
		.BorderBackgroundColor_Lambda([this](void)->FSlateColor {return GetTerrainColor(); })
		[
			// The widget to switch between terrains
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.MaxHeight(80.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				// The Rock terrain
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MaxWidth(80.0f)
				.Padding(FMargin(1.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SButton)
					.OnClicked_Raw               (this, &STerrainSelector::OnRockTerrainClick)
					.ForegroundColor_Lambda      ([this](void)->FSlateColor { return GetButtonColor(0); })
					.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor { return GetButtonColor(0); })
					.ToolTipText_Lambda          ([this](void)->FText       { return FText::FromString(TEXT(
						"Click on this button to modify rock terrain related values.")); })
					[
						TSharedRef<SWidget>(SNew(SImage)
						.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.RockTerrain")))
					]
				]
				// The Sand terrain
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MaxWidth(80.0f)
				.Padding(FMargin(1.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SButton)
					.OnClicked_Raw               (this, &STerrainSelector::OnSandTerrainClick)
					.ForegroundColor_Lambda      ([this](void)->FSlateColor { return GetButtonColor(1); })
					.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor { return GetButtonColor(1); })
					.ToolTipText_Lambda          ([this](void)->FText       { return FText::FromString(TEXT(
						"Click on this button to modify sand terrain related values.")); })
					[
						TSharedRef<SWidget>(SNew(SImage)
						.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.SandTerrain")))
					]
				]
				// The Water terrain
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MaxWidth(80.0f)
				.Padding(FMargin(1.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SButton)
					.OnClicked_Raw               (this, &STerrainSelector::OnWaterTerrainClick)
					.ForegroundColor_Lambda      ([this](void)->FSlateColor { return GetButtonColor(2); })
					.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor { return GetButtonColor(2); })
					.ToolTipText_Lambda          ([this](void)->FText       { return FText::FromString(TEXT(
						"Click on this button to modify water terrain related values.")); })
					[
						TSharedRef<SWidget>(SNew(SImage)
						.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.WaterTerrain")))
					]
				]
				// The Neutral terrain
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MaxWidth(80.0f)
				.Padding(FMargin(1.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SButton)
					.OnClicked_Raw               (this, &STerrainSelector::OnNeutralTerrainClick)
					.ForegroundColor_Lambda      ([this](void)->FSlateColor { return GetButtonColor(3); })
					.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor { return GetButtonColor(3); })
					.ToolTipText_Lambda          ([this](void)->FText       { return FText::FromString(TEXT(
						"Click on this button to modify neutral terrain related values.")); })
					[
						TSharedRef<SWidget>(SNew(SImage)
						.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.NeutralTerrain")))
					]
				]
			]
			// Slot for the 'Apply' button
			+ SVerticalBox::Slot()
			.MaxHeight(40.0f)
			.AutoHeight()
			.Padding(FMargin(0.0f, 30.0f, 0.0f, 0.0f))
			[
				SNew(SButton)
				.DesiredSizeScale(FVector2D(50.0f, 40.0f))
				.VAlign(EVerticalAlignment  ::VAlign_Center)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.ForegroundColor_Lambda      ([this](void)->FSlateColor { return GetButtonForegroundColor(); })
				.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor { return GetButtonBackgroundColor(); })
				.OnClicked_Raw     (this, &STerrainSelector::ApplyChanges)
				.Text_Lambda	   ([this](void)->FText { return FText::FromString(TEXT("Apply"));        })
				.ToolTipText_Lambda([this](void)->FText { return FText::FromString(TEXT(
					"Applies all current changes to the editor world.")); })
				
			]
			// Slot for the 'Reset' button
			+ SVerticalBox::Slot()
			.MaxHeight(40.0f)
			.AutoHeight()
			.Padding(FMargin(0.0f, 00.0f, 0.0f, 30.0f))
			[
				SNew(SButton)
				.DesiredSizeScale(FVector2D(50.0f, 40.0f))
				.VAlign(EVerticalAlignment  ::VAlign_Center)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.ForegroundColor_Lambda      ([this](void)->FSlateColor { return GetButtonForegroundColor(); })
				.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor { return GetButtonBackgroundColor(); })
				.OnClicked_Raw     (this, &STerrainSelector::ResetChanges)
				.Text_Lambda       ([this](void)->FText { return FText::FromString(TEXT("Reset"));        })
				.ToolTipText_Lambda([this](void)->FText { return FText::FromString(TEXT(
					"Resets all values to the values contained in the editor world.")); })
			]
			// All terrain widgets
			+ SVerticalBox::Slot()
			.AutoHeight()
			.MaxHeight(2000.0f)
			[
				SNew(SWidgetSwitcher)
				.WidgetIndex(this, &STerrainSelector::GetCurrentTabIndex)
				+ SWidgetSwitcher::Slot()[rockTerrainWidget.ToSharedRef()    ]
				+ SWidgetSwitcher::Slot()[sandTerrainWidget.ToSharedRef()    ]
				+ SWidgetSwitcher::Slot()[waterTerrainWidget.ToSharedRef()   ]
				+ SWidgetSwitcher::Slot()[neutralTerrainWidget.ToSharedRef() ]
			]
		]
	];
}

FReply STerrainSelector::OnRockTerrainClick()
{
	m_tabIndex = 0;
	return FReply::Handled();
}

FReply STerrainSelector::OnSandTerrainClick()
{
	m_tabIndex = 1;
	return FReply::Handled();
}

FReply STerrainSelector::OnWaterTerrainClick()
{
	m_tabIndex = 2;
	return FReply::Handled();
}

FReply STerrainSelector::OnNeutralTerrainClick()
{
	m_tabIndex = 3;
	return FReply::Handled();
}

int32 STerrainSelector::GetCurrentTabIndex() const
{
	return m_tabIndex;
}

FLinearColor STerrainSelector::GetTerrainColor() const
{
	switch(m_tabIndex)
	{
		case 0:  return FLinearColor(0.458f, 0.298f, 0.141f, 1.0f); // Rock
		case 1:  return FLinearColor(0.964f, 0.776f, 0.231f, 1.0f); // Sand
		case 2:  return FLinearColor(0.000f, 0.576f, 0.737f, 1.0f); // Water
		case 3:  return FLinearColor(0.400f, 0.400f, 0.400f, 1.0f); // Neutral
		default: break;
	}

	return FLinearColor(0.400f, 0.400f, 0.400f, 1.0f); // Neutral
}

FLinearColor STerrainSelector::GetButtonColor(int32 index) const
{
	if (index == m_tabIndex)
	{
		return FLinearColor(0.243f, 0.243f, 0.243f, 1.0f);
	}

	return FLinearColor(0.086f, 0.086f, 0.086f, 1.0f);
}

FLinearColor STerrainSelector::GetButtonBackgroundColor() const
{
	return FLinearColor(0.243f, 0.243f, 0.243f, 1.0f);
}

FLinearColor STerrainSelector::GetButtonForegroundColor() const
{
	return FLinearColor(0.800f, 0.800f, 0.800f, 1.0f);
}

FReply STerrainSelector::ApplyChanges()
{
	for (auto& widget : m_widgets) {
		widget.Get()->ApplyChanges();
	}

	CameraData data;
	data.m_maxArmLenghtValue	     = m_cameraMaxArmLenght.Get()->GetValue();
	data.m_maxTimeFromLastInputValue = m_cameraMaxTimeFromLastInput.Get()->GetValue();

	// Commits the camera data changes
	GAttunedTool::Get()->GetModel()->CommitChanges(data);

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Registered : %lf"), data.m_maxArmLenghtValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Registered : %lf"), data.m_maxTimeFromLastInputValue);

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Changes applied."));
	return FReply::Handled();
}

FReply STerrainSelector::ResetChanges()
{
	for (auto& widget : m_widgets) {
		widget.Get()->ResetChanges();
	}

	CameraData data;
	GAttunedTool::Get()->GetModel()->DeserializeData<CameraData>(data);

	m_cameraMaxArmLenght.Get()->SetValue(data.m_maxArmLenghtValue);
	m_cameraMaxTimeFromLastInput.Get()->SetValue(data.m_maxTimeFromLastInputValue);

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Changes reset."));
	return FReply::Handled();
}

void STerrainSelector::OnCameraMaxArmLenghtChange(float value)
{
	UE_LOG(LogTemp, Log, TEXT("[Attuned] Value changes : %lf"), value);

	// TODO
}

void STerrainSelector::OnCameraMaxTimeFromLastInputChange(float value)
{
	UE_LOG(LogTemp, Log, TEXT("[Attuned] Value changes : %lf"), value);

	// TODO
}
#undef LOCTEXT_NAMESPACE