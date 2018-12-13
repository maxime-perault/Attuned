// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AttunedToolStyle.h"

class FAttunedToolCommands : public TCommands<FAttunedToolCommands>
{
public:

	FAttunedToolCommands()
		: TCommands<FAttunedToolCommands>(TEXT("AttunedTool"), NSLOCTEXT("Contexts", "AttunedTool", "AttunedTool Plugin"), NAME_None, FAttunedToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};