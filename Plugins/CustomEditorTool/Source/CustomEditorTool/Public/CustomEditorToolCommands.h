// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CustomEditorToolStyle.h"

class FCustomEditorToolCommands : public TCommands<FCustomEditorToolCommands>
{
public:

	FCustomEditorToolCommands()
		: TCommands<FCustomEditorToolCommands>(TEXT("CustomEditorTool"), NSLOCTEXT("Contexts", "CustomEditorTool", "CustomEditorTool Plugin"), NAME_None, FCustomEditorToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
	TSharedPtr<FUICommandInfo>OpenRecentBlueprints;
};
