/// \file       SRockMomemtumSettings.cpp
/// \date       22/01/2019
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/Terrain/SRockMomemtumSettings.h"

#include <SButton.h>
#include <SCheckBox.h>
#include <STextBlock.h>
#include <SExpandableArea.h>
#include <SWidgetSwitcher.h>
#include <Widgets/Layout/SScrollBox.h>

#include "Global/GAttunedTool.h"

#define LOCTEXT_NAMESPACE "SRockMomemtumSettings"

void SRockMomemtumSettings::Construct(const FArguments& InArgs)
{
	m_activeCheckBox = SNew(SCheckBox)
		.HAlign             (EHorizontalAlignment::HAlign_Right)
		.IsChecked          (this, &SRockMomemtumSettings::IsActiveCheckBoxChecked)
		.OnCheckStateChanged(this, &SRockMomemtumSettings::OnActiveCheckBoxCheckStateChanged);

	m_squareCheckBox = SNew(SCheckBox)
		.HAlign             (EHorizontalAlignment::HAlign_Right)
		.IsChecked          (this, &SRockMomemtumSettings::IsSquareCheckBoxChecked)
		.OnCheckStateChanged(this, &SRockMomemtumSettings::OnSquareCheckBoxCheckStateChanged);

	m_minMomemtum = SNew(SSpinBox<float>)
		.MinValue		(RockMomemtumData::MinMomemtumMinValue)
		.MaxValue		(1.0f)
		.Value			(this, &SRockMomemtumSettings::MinMomemtumValue)
		.OnValueChanged	(this, &SRockMomemtumSettings::OnMinMomemtumValueChanged);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding   (FMargin(0.0f, 10.0f, 0.0f, 0.0f))
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed              (false)
			.BorderBackgroundColor_Lambda    ([this](void)->FSlateColor { return GetExpandableBorderColor(); })
			.BodyBorderBackgroundColor_Lambda([this](void)->FSlateColor { return GetExpandableBodyColor  (); })
			.HeaderContent()

			[
				SNew(STextBlock)
				.Text(NSLOCTEXT("SRockMomemtumSettings", "CategoryHeader", "Rock Momemtum"))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 10))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("SRockMomemtumSettings", "Momemtum1", "Is active"))
					]
				    + SHorizontalBox::Slot()
				    [
						m_activeCheckBox.ToSharedRef()
				    ]
				]
				+
				SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("SRockMomemtumSettings", "Momemtum2", "Is square"))
					]
				    + SHorizontalBox::Slot()
				    [
						m_squareCheckBox.ToSharedRef()
				    ]
				]
				+
				SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("SRockMomemtumSettings", "Momemtum3", "Min momemtum"))
					]
				    + SHorizontalBox::Slot()
				    [
						m_minMomemtum.ToSharedRef()
				    ]
				]
			]
		]
	];
}

FSlateColor SRockMomemtumSettings::GetExpandableBodyColor() const
{
	return FSlateColor(FLinearColor(1.00f, 0.00f, 0.00f, 1.00f));
}

FSlateColor SRockMomemtumSettings::GetExpandableBorderColor() const
{
	return FSlateColor(FLinearColor(0.18f, 0.18f, 0.18f, 1.00f));
}

void SRockMomemtumSettings::OnActiveCheckBoxCheckStateChanged(ECheckBoxState state)
{
	RockMomemtumData data;
	data.m_bActiveMomemtum  = (state == ECheckBoxState::Checked) ? true : false;
	data.m_bSquareMomemtum  = m_squareCheckBox->IsChecked();
	data.m_minMomemtumValue = m_minMomemtum->GetValue();

	GAttunedTool::Get()->UpdateModel(data);
}

void SRockMomemtumSettings::OnSquareCheckBoxCheckStateChanged(ECheckBoxState state)
{
	RockMomemtumData data;
	data.m_bActiveMomemtum  = m_activeCheckBox->IsChecked();
	data.m_bSquareMomemtum  = (state == ECheckBoxState::Checked) ? true : false;
	data.m_minMomemtumValue = m_minMomemtum->GetValue();

	GAttunedTool::Get()->UpdateModel(data);
}

void SRockMomemtumSettings::OnMinMomemtumValueChanged(float value)
{
	RockMomemtumData data;
	data.m_bActiveMomemtum  = m_activeCheckBox->IsChecked();
	data.m_bSquareMomemtum  = m_squareCheckBox->IsChecked();
	data.m_minMomemtumValue = value;

	GAttunedTool::Get()->UpdateModel(data);
}

ECheckBoxState SRockMomemtumSettings::IsActiveCheckBoxChecked() const
{
	const RockMomemtumData* cache = GAttunedTool::Get()->GetModel<RockMomemtumData>();
	return (cache->m_bActiveMomemtum) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SRockMomemtumSettings::IsSquareCheckBoxChecked() const
{
	const RockMomemtumData* cache = GAttunedTool::Get()->GetModel<RockMomemtumData>();
	return (cache->m_bSquareMomemtum) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

float SRockMomemtumSettings::MinMomemtumValue() const
{
	const RockMomemtumData* cache = GAttunedTool::Get()->GetModel<RockMomemtumData>();
	return cache->m_minMomemtumValue;
}

#undef LOCTEXT_NAMESPACE