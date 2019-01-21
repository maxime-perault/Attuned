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
#include <SlateApplication.h>

#include <SBox.h>
#include <SImage.h>
#include <SDockTab.h>
#include <STextBlock.h>
#include <STextComboBox.h>

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
		.SetDisplayName(LOCTEXT("FAttunedToolTabTitle", "Tweak Box"))
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
	source.Empty();
	source.Add(MakeShared<FString>("Antoine"));
	source.Add(MakeShared<FString>("Maxime"));
	source.Add(MakeShared<FString>("Paul"));
	source.Add(MakeShared<FString>("Vincent"));
	source.Add(MakeShared<FString>("Yannis"));
	
	b = FButtonStyle::GetDefault();
	b = FCoreStyle::Get().GetWidgetStyle< FButtonStyle >("Button");
	b.Normal.TintColor  = FSlateColor(FLinearColor(0.243f, 0.243f, 0.243f));
	b.Hovered.TintColor = FSlateColor(FLinearColor(0.343f, 0.343f, 0.343f));

	const ProfilePreferenceData* data = GAttunedTool::Get()->GetModel<ProfilePreferenceData>();

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
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.MaxHeight(100.0f)
				.AutoHeight()
				[
					SNew(SHorizontalBox) 
					+ SHorizontalBox::Slot()
					.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
					.VAlign(EVerticalAlignment::VAlign_Center)
	
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("AttunedTool", "Profile", "Current profile : "))
					]
					+ SHorizontalBox::Slot()
					.MaxWidth(330.0f)
					.Padding (FMargin(4.0f, 4.0f, 4.0f, 4.0f))
					[
						SNew(STextComboBox)
						.ButtonStyle		  (&b)
						.OptionsSource		  (&source)
						.InitiallySelectedItem(source[data->m_profileIndex])
						.ColorAndOpacity	  (FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f)))
						//.OnSelectionChanged   (this, &FAttunedToolModule::OnSelectionChanged)
						.OnSelectionChanged_Raw(this, &FAttunedToolModule::OnSelectionChanged)
					]
				]
				+ SVerticalBox::Slot()
				.MaxHeight(5.0f)
				.AutoHeight()
				.Padding(FMargin(0.0f, 5.0f, 0.0f, 5.0f))
				[
					SNew(SSeparator)
					.Thickness(5.0f)
			
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBox)
					[
						SNew(STerrainSelector)
					]
				]
			]
		];
}

void FAttunedToolModule::OnSelectionChanged(TSharedPtr<FString> s, ESelectInfo::Type t)
{
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] New Selection : %s"), **s);

	int i = 0;
	for (auto ptr : source)
	{
		if (ptr == s)
		{
			ProfilePreferenceData data{};
			data.m_profileIndex = i;
			data.m_profileName  = *s;

			GAttunedTool::Get()->UpdateModel(data);
			GAttunedTool::Get()->CommitProfilePreference();

			GAttunedTool::Get()->InvalidateAllCaches();
			GAttunedTool::Get()->RevertModel();
	
			break;
		}
		++i;
	}
}

void FAttunedToolModule::PluginButtonClicked()
{
	auto dockTab      = FGlobalTabmanager::Get()->InvokeTab(AttunedToolTabName);
	auto parentWindow = dockTab->GetParentWindow();

	if (parentWindow.IsValid())
	{
		parentWindow->Resize       (FVector2D(335.0f, 600.0f));
		parentWindow->SetSizingRule(ESizingRule::FixedSize);
		parentWindow->SetCanTick   (true);
	}
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