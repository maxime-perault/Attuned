/// \file       SSandTerrainSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/Terrain/SSandTerrainSettings.h"
#include "Widget/STerrainCommonSettings.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SWidgetSwitcher.h>

#define LOCTEXT_NAMESPACE "SSandTerrainSettings"

void SSandTerrainSettings::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(STerrainCommonSettings)
	];
}

#undef LOCTEXT_NAMESPACE