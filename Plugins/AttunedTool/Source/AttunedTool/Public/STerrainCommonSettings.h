/// \file       STerrainCommonSettings.h
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_S_TERRAIN_COMMON_SETTINGS_H_
#define ATTUNED_TOOL_S_TERRAIN_COMMON_SETTINGS_H_

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

/// \brief Common settings for all terrains
/// \class STerrainCommonSettings
class STerrainCommonSettings : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(STerrainCommonSettings) {}
	SLATE_END_ARGS()

public:

	void Construct(const FArguments& InArgs);

private:

};

#endif // ATTUNED_TOOL_S_TERRAIN_COMMON_SETTINGS_H_