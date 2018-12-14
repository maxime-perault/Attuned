/// \file       GAttunedTool.h
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_G_ATTUNED_TOOL_H_
#define ATTUNED_TOOL_G_ATTUNED_TOOL_H_

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

/// \brief Global 
/// \class GAttunedTool
class GAttunedTool
{
public:

	// TODO

private:

	friend class AttunedTool;

	/// \brief Initialize the singleton
	static void Initialize();

	/// \brief Destroy the singleton
	static void Destroy   ();

	/// \brief Returns a reference on the internal smart pointer
	static TUniquePtr<GAttunedTool>& Get();

private:

	static TUniquePtr<GAttunedTool> s_attuned_tool;

};

#endif // ATTUNED_TOOL_G_ATTUNED_TOOL_H_