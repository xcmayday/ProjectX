// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomEditorToolCommands.h"

#define LOCTEXT_NAMESPACE "FCustomEditorToolModule"

void FCustomEditorToolCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "CustomEditorTool", "Execute CustomEditorTool action", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(OpenRecentBlueprints, "Open Recent Blueprints", "Edit the Recent Blueprint ", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
