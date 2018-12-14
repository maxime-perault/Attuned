/// \file       SNeutralTerrainSettings.h
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_S_NEUTRAL_TERRAIN_SETTINGS_H_
#define ATTUNED_TOOL_S_NEUTRAL_TERRAIN_SETTINGS_H_

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

/// \brief Settings for the neutral terrain
/// \class SNeutralTerrainSettings
class SNeutralTerrainSettings : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SNeutralTerrainSettings) {}
	SLATE_END_ARGS()

public:

	void Construct(const FArguments& InArgs);

private:

};

#endif // ATTUNED_TOOL_S_NEUTRAL_TERRAIN_SETTINGS_H_