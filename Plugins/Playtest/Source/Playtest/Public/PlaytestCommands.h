// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "PlaytestStyle.h"

class FPlaytestCommands : public TCommands<FPlaytestCommands>
{
public:

	FPlaytestCommands()
		: TCommands<FPlaytestCommands>(TEXT("Playtest"), NSLOCTEXT("Contexts", "Playtest", "Playtest Plugin"), NAME_None, FPlaytestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
