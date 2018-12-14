/// \file       SRockTerrainSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/Terrain/SRockTerrainSettings.h"
#include "Widget/STerrainCommonSettings.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SWidgetSwitcher.h>

#define LOCTEXT_NAMESPACE "SRockTerrainSettings"

void SRockTerrainSettings::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(STerrainCommonSettings)
	];
}

#undef LOCTEXT_NAMESPACE