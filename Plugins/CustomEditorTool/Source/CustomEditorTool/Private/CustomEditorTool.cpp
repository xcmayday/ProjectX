// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomEditorTool.h"
#include "CustomEditorToolStyle.h"
#include "CustomEditorToolCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "LevelEditorMenuContext.h"
#include "LevelEditor.h"
#include "Engine/LevelScriptBlueprint.h"
#include "Engine/World.h"
#include <EditorStyleSet.h>
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"

static const FName CustomEditorToolTabName("CustomEditorTool");
//static const FName RecentBlueprint("RecentBlueprint");
DEFINE_LOG_CATEGORY_STATIC(CustomEditorToolModule, Log, All);
#define LOCTEXT_NAMESPACE "FCustomEditorToolModule"




void FCustomEditorToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCustomEditorToolStyle::Initialize();
	FCustomEditorToolStyle::ReloadTextures();

	FCustomEditorToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCustomEditorToolCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FCustomEditorToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCustomEditorToolModule::RegisterMenus));
}

void FCustomEditorToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCustomEditorToolStyle::Shutdown();

	FCustomEditorToolCommands::Unregister();
}

void FCustomEditorToolModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions,ByAcung"),
							FText::FromString(TEXT("FCustomEditorToolModule::PluginButtonClicked()")),
							FText::FromString(TEXT("CustomEditorTool.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FCustomEditorToolModule::MakeOpenRecentBPClassMenu(UToolMenu* InMenu)
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	// Configure filter for asset picker
	FAssetPickerConfig Config;
	Config.Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Config.Filter.ClassNames.Add(UMaterial::StaticClass()->GetFName());
	//UE_LOG(CustomEditorToolModule, Warning, TEXT("MakeOpenRecentBPClassMenu---%s"), *UBlueprint::StaticClass()->GetFName());
	UE_LOG(CustomEditorToolModule, Warning, TEXT("MakeOpenRecentBPClassMenu"));
	Config.InitialAssetViewType = EAssetViewType::List;
	Config.OnAssetSelected = FOnAssetSelected::CreateStatic(&FCustomEditorToolModule::OnBPSelected);
	Config.bAllowDragging = false;
	// Don't show stuff in Engine
	Config.Filter.PackagePaths.Add("/Game");
	Config.Filter.bRecursivePaths = true;
	for (FName it:Config.Filter.ClassNames)
	{
		UE_LOG(CustomEditorToolModule, Warning, TEXT("MakeOpenRecentBPClassMenu---ClassName=%s"),*it.ToString());
	}
	TSharedRef<SWidget> Widget =
		SNew(SBox)
		.WidthOverride(300.f)
		.HeightOverride(300.f)
		[
			ContentBrowserModule.Get().CreateAssetPicker(Config)
			
		];


	{
		FToolMenuSection& Section = InMenu->AddSection("Browse", LOCTEXT("BrowseHeader", "Browse"));
		Section.AddEntry(FToolMenuEntry::InitWidget("PickClassWidget", Widget, FText::GetEmpty()));
		
	}
}

void FCustomEditorToolModule::OnBPSelected(const struct FAssetData& AssetData)
{
	UBlueprint* SelectedBP = Cast<UBlueprint>(AssetData.GetAsset());
	if (SelectedBP)
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(SelectedBP);
	}

}

void FCustomEditorToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCustomEditorToolCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorToolCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		FToolMenuSection& Section = Menu->FindOrAddSection("Misc");
		FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorToolCommands::Get().PluginAction));
		Entry.SetCommandList(PluginCommands);
	}
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.OpenBlueprint");
		FToolMenuSection& Section = Menu->FindOrAddSection("BlueprintClass");
		//Section.AddMenuEntry(FCustomEditorToolCommands::Get().OpenRecentBlueprints, LOCTEXT("OpenRecentBlueprints", "Open Recent Blueprint Class..."));
		FSlateIcon RecentBPIcon(FCustomEditorToolStyle::GetStyleSetName(),"LevelEditor.OpenRecentClassBlueprint");
		Section.AddSubMenu("OpenRecentBlueprintClass",
			LOCTEXT("OpenRecentBlueprintClassSubMenu", "Open Recent Blueprint Class..."),
			LOCTEXT("OpenRecentBlueprintClassSubMenu_ToolTip", "Open Recent Blueprint Class in this project"),
			FNewToolMenuDelegate::CreateStatic(&FCustomEditorToolModule::MakeOpenRecentBPClassMenu), false, RecentBPIcon);


	}
	{
		/*UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.OpenBlueprint");
		FToolMenuSection& NewSection = Menu->AddSection("RecentBlueprints", LOCTEXT("RecentBlueprints", "Open Recent Blueprint Class..."));
		NewSection.AddMenuEntry(FCustomEditorToolCommands::Get().OpenRecentBlueprints);*/
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomEditorToolModule, CustomEditorTool)