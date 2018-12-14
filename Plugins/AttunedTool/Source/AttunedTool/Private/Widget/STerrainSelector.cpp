/// \file       STerrainSelector.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/STerrainSelector.h"

#include <SButton.h>
#include <SWidgetSwitcher.h>

#define LOCTEXT_NAMESPACE "STerrainSelector"

void STerrainSelector::Construct(const FArguments& InArgs)
{
	m_tabIndex = 0;

	ChildSlot
	[
		// The widget to switch between terrains
		// Rock - Sand - Water - Neutral
		SNew(SWidgetSwitcher)
		.WidgetIndex(this, &STerrainSelector::GetCurrentTabIndex)
		+ SWidgetSwitcher::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Rock"))
			.DesiredSizeScale(FVector2D(2.5, 2.5))
			.OnClicked_Raw(this, &STerrainSelector::OnRockTerrainClick)
		]
		+ SWidgetSwitcher::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Sand"))
			.DesiredSizeScale(FVector2D(2.5, 2.5))
			.OnClicked_Raw(this, &STerrainSelector::OnSandTerrainClick)
		]
		+ SWidgetSwitcher::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Water"))
			.DesiredSizeScale(FVector2D(2.5, 2.5))
			.OnClicked_Raw(this, &STerrainSelector::OnWaterTerrainClick)
		]
		+ SWidgetSwitcher::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Neutral"))
			.DesiredSizeScale(FVector2D(2.5, 2.5))
			.OnClicked_Raw(this, &STerrainSelector::OnNeutralTerrainClick)
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

#undef LOCTEXT_NAMESPACE