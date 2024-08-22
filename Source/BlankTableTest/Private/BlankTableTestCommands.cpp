// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlankTableTestCommands.h"

#define LOCTEXT_NAMESPACE "FBlankTableTestModule"

void FBlankTableTestCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "BlankTableTest", "Execute BlankTableTest action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
