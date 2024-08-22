// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "BlankTableTestStyle.h"

class FBlankTableTestCommands : public TCommands<FBlankTableTestCommands>
{
public:

	FBlankTableTestCommands()
		: TCommands<FBlankTableTestCommands>(TEXT("BlankTableTest"), NSLOCTEXT("Contexts", "BlankTableTest", "BlankTableTest Plugin"), NAME_None, FBlankTableTestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
