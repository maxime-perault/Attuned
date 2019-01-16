/// \file       SSandTerrainSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/Terrain/SSandTerrainSettings.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SWidgetSwitcher.h>
#include <Widgets/Layout/SScrollBox.h>

#define LOCTEXT_NAMESPACE "SSandTerrainSettings"

void SSandTerrainSettings::Construct(const FArguments& InArgs)
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

void SSandTerrainSettings::ApplyChanges()
{
	m_terrainCommonSettings->ApplyChanges();
}

void SSandTerrainSettings::ResetChanges()
{
	m_terrainCommonSettings->ResetChanges();
}

#undef LOCTEXT_NAMESPACE