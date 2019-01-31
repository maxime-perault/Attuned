// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PlaytestCommands.h"

#define LOCTEXT_NAMESPACE "FPlaytestModule"

void FPlaytestCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Playtest", "Execute Playtest action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
