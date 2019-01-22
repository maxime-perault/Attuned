/// \file       SWaterTerrainSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/Terrain/SWaterTerrainSettings.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SWidgetSwitcher.h>
#include <Widgets/Layout/SScrollBox.h>

#define LOCTEXT_NAMESPACE "SWaterTerrainSettings"

void SWaterTerrainSettings::Construct(const FArguments& InArgs)
{
	m_terrainCommonSettings = SNew(STerrainCommonSettings)
		.terrainType			   (STerrainCommonSettings::ETerrainType::Water)
		.cameraMaxArmLenght        (InArgs._cameraMaxArmLenght)
		.cameraMaxTimeFromLastInput(InArgs._cameraMaxTimeFromLastInput);;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.MaxHeight(290.0f)
		.AutoHeight()
		[
			SNew(SScrollBox)
			.AllowOverscroll(EAllowOverscroll::Yes)
			.IsEnabled(true)
			+ SScrollBox::Slot()
			[
				m_terrainCommonSettings.ToSharedRef()
			]
		]
	];
}

void SWaterTerrainSettings::ApplyChanges()
{
	m_terrainCommonSettings->ApplyChanges();
}

void SWaterTerrainSettings::ResetChanges()
{
	m_terrainCommonSettings->ResetChanges();
}

#undef LOCTEXT_NAMESPACE