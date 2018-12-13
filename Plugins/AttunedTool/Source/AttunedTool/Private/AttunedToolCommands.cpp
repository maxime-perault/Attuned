// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AttunedToolCommands.h"

#define LOCTEXT_NAMESPACE "FAttunedToolModule"

void FAttunedToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "AttunedTool", "Bring up AttunedTool window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
