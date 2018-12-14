/// \file       GAttunedTool.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Global/GAttunedTool.h"

#define LOCTEXT_NAMESPACE "GAttunedTool"

/* static */ TUniquePtr<GAttunedTool> GAttunedTool::s_attuned_tool;

/* static */ void GAttunedTool::Initialize()
{
	if (!s_attuned_tool)
	{
		s_attuned_tool = MakeUnique<GAttunedTool>();
	}
}

/* static */ void GAttunedTool::Destroy()
{
	if (s_attuned_tool)
	{
		s_attuned_tool.Reset();
	}
}

/* static */ TUniquePtr<GAttunedTool>& GAttunedTool::Get()
{
	if (!s_attuned_tool)
	{
		UE_LOG(LogTemp, Warning, TEXT("The global tool pointer is nullptr."));
	}

	return s_attuned_tool;
}

#undef LOCTEXT_NAMESPACE