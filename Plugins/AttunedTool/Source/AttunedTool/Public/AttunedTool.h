/// \file       AttunedTool.h
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_ATTUNED_TOOL_H
#define ATTUNED_TOOL_ATTUNED_TOOL_H

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include <STextComboBox.h>

class FMenuBuilder;
class FToolBarBuilder;

class FAttunedToolModule : public IModuleInterface
{
public:

	/// \brief IModuleInterface implementation 
	virtual void StartupModule () override;
	virtual void ShutdownModule() override;
	
	/// \brief This function will be bound to the command that will bring up the plugin window
	void PluginButtonClicked();
	
private:

	void AddMenuExtension   (FMenuBuilder& Builder);
	void AddToolbarExtension(FToolBarBuilder& Builder);
	
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	// TMP
	void OnSelectionChanged(TSharedPtr<FString> s, ESelectInfo::Type t);

private:

	FButtonStyle b;
	TArray<TSharedPtr<FString>> source;
	TSharedPtr<class FUICommandList> PluginCommands;
};

#endif // ATTUNED_TOOL_ATTUNED_TOOL_H