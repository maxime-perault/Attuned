// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.



#include "AttunedTool.h"
#include "AttunedToolStyle.h"
#include "AttunedToolCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widget/STerrainSelector.h"
#include "Attuned/Public/APluginActor.h"

static const FName AttunedToolTabName("AttunedTool");

#define LOCTEXT_NAMESPACE "FAttunedToolModule"

void FAttunedToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FAttunedToolStyle::Initialize();
	FAttunedToolStyle::ReloadTextures();

	FAttunedToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAttunedToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAttunedToolModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FAttunedToolModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FAttunedToolModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AttunedToolTabName, FOnSpawnTab::CreateRaw(this, &FAttunedToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FAttunedToolTabTitle", "AttunedTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FAttunedToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FAttunedToolStyle::Shutdown();

	FAttunedToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AttunedToolTabName);
}

TSharedRef<SDockTab> FAttunedToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FAttunedToolModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("AttunedTool.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STerrainSelector)
			]
		];
}

void FAttunedToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(AttunedToolTabName);
}

void FAttunedToolModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FAttunedToolCommands::Get().OpenPluginWindow);
}

void FAttunedToolModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FAttunedToolCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAttunedToolModule, AttunedTool)