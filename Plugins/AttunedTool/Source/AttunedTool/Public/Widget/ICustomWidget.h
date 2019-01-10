/// \file       ICustomWidget.h
/// \date       09/01/2019
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_I_CUSTOM_WIDGET_H_
#define ATTUNED_TOOL_I_CUSTOM_WIDGET_H_

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

/// \brief Interface for all custom widgets
/// \class ICustomWidget
class ICustomWidget : public SCompoundWidget
{
public:

	virtual void ApplyChanges();
	virtual void ResetChanges();
};

#endif // ATTUNED_TOOL_I_CUSTOM_WIDGET_H_