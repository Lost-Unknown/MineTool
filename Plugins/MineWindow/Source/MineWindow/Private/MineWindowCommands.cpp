// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineWindowCommands.h"

#define LOCTEXT_NAMESPACE "FMineWindowModule"

void FMineWindowCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MineWindow", "Bring up Mine Sweeper window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
