// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FCustomEditorToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

	static void MakeOpenRecentBPClassMenu(UToolMenu* InMenu);
	static void OnBPSelected(const struct FAssetData& AssetData);

	
private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
	
};
