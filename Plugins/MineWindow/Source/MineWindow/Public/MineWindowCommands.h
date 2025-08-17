// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "MineWindowStyle.h"

class FMineWindowCommands : public TCommands<FMineWindowCommands>
{
public:

	FMineWindowCommands()
		: TCommands<FMineWindowCommands>(TEXT("MineWindow"), NSLOCTEXT("Contexts", "MineWindow", "MineWindow Plugin"), NAME_None, FMineWindowStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};