/// \file       SSandTerrainSettings.h
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_S_SAND_TERRAIN_SETTINGS_H_
#define ATTUNED_TOOL_S_SAND_TERRAIN_SETTINGS_H_

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

#include "Widget/ICustomWidget.h"
#include "Widget/STerrainCommonSettings.h"

/// \brief Settings for the sand terrain
/// \class SSandTerrainSettings
class SSandTerrainSettings : public ICustomWidget
{
public:

	SLATE_BEGIN_ARGS(SSandTerrainSettings) {}
		SLATE_ARGUMENT(TSharedPtr<SSpinBox<float>>, cameraMaxArmLenght)
		SLATE_ARGUMENT(TSharedPtr<SSpinBox<float>>, cameraMaxTimeFromLastInput)
	SLATE_END_ARGS()

public:

	void Construct(const FArguments& InArgs);

	void ApplyChanges() final;
	void ResetChanges() final;

private:

	TSharedPtr<STerrainCommonSettings> m_terrainCommonSettings;
};

#endif // ATTUNED_TOOL_S_SAND_TERRAIN_SETTINGS_H_