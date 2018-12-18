/// \file       AttunedTool.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "AttunedTool.h"
#include "AttunedToolStyle.h"
#include "AttunedToolCommands.h"

#include "Global/GAttunedTool.h"
#include "Widget/STerrainSelector.h"

#include <LevelEditor.h>

#include <SBox.h>
#include <SImage.h>
#include <SDockTab.h>
#include <STextBlock.h>

#include <Framework/MultiBox/MultiBoxBuilder.h>

static const FName AttunedToolTabName("AttunedTool");

#define LOCTEXT_NAMESPACE "FAttunedToolModule"

void FAttunedToolModule::StartupModule()
{
	// Initializes the global tool class
	GAttunedTool::Initialize();

	// Loads styles
	FAttunedToolStyle::Initialize();
	FAttunedToolStyle::ReloadTextures();

	// Creates the commands
	FAttunedToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FAttunedToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAttunedToolModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

		// Placing the plugin button right after the play button in the editor (Game hook)
		ToolbarExtender->AddToolBarExtension("Game", EExtensionHook::After, PluginCommands, 
			FToolBarExtensionDelegate::CreateRaw(this, &FAttunedToolModule::AddToolbarExtension));
		
		// Adds the plugin button
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	// Registers the window
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AttunedToolTabName, 
		FOnSpawnTab::CreateRaw(this, &FAttunedToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FAttunedToolTabTitle", "AttunedTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FAttunedToolModule::ShutdownModule()
{
	// Destroys the internal pointer
	GAttunedTool::Destroy();

	FAttunedToolStyle::Shutdown();
	FAttunedToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AttunedToolTabName);
}

TSharedRef<SDockTab> FAttunedToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	// To make the code better, the tool has been split up into several
	// custom widgets.
	return 
		SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				TSharedRef<SWidget>(SNew(SImage)
				.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.TabColor")))
			]
			+ SOverlay::Slot()
			[
				SNew(SBox)
				[
					SNew(STerrainSelector)
				]
			]
		];

	// ([this](void)->FLinearColor {return FLinearColor(0.243f, 0.243f, 0.243f, 1.0f); })
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