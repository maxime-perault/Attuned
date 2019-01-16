/// \file       SRockTerrainSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/Terrain/SRockTerrainSettings.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SWidgetSwitcher.h>
#include <Widgets/Layout/SScrollBox.h>

#define LOCTEXT_NAMESPACE "SRockTerrainSettings"

void SRockTerrainSettings::Construct(const FArguments& InArgs)
{
	m_terrainCommonSettings = SNew(STerrainCommonSettings)
		.cameraMaxArmLenght        (InArgs._cameraMaxArmLenght)
		.cameraMaxTimeFromLastInput(InArgs._cameraMaxTimeFromLastInput);

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		[
			m_terrainCommonSettings.ToSharedRef()
		]
	];
}

void SRockTerrainSettings::ApplyChanges()
{
	m_terrainCommonSettings->ApplyChanges();
}

void SRockTerrainSettings::ResetChanges()
{
	m_terrainCommonSettings->ResetChanges();
}

#undef LOCTEXT_NAMESPACE