/// \file       SRockTerrainSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/Terrain/SRockTerrainSettings.h"
#include "Widget/Terrain/SRockMomemtumSettings.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SWidgetSwitcher.h>
#include <Widgets/Layout/SScrollBox.h>

#define LOCTEXT_NAMESPACE "SRockTerrainSettings"

void SRockTerrainSettings::Construct(const FArguments& InArgs)
{
	m_terrainCommonSettings = SNew(STerrainCommonSettings)
		.terrainType			   (STerrainCommonSettings::ETerrainType::Rock)
		.cameraMaxArmLenght        (InArgs._cameraMaxArmLenght)
		.cameraMaxTimeFromLastInput(InArgs._cameraMaxTimeFromLastInput);

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
			+ SScrollBox::Slot()
			[
				SNew(SRockMomemtumSettings)
			]
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