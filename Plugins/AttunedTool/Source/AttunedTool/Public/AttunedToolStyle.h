/// \file       AttunedToolStyle.h
/// \date       17/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_ATTUNED_TOOL_STYLE_H
#define ATTUNED_ATTUNED_TOOL_STYLE_H

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/// \brief Manages the textures, fonts of the slate renderer
/// \class FAttunedToolStyle
class FAttunedToolStyle
{
public:

	static void Initialize();
	static void Shutdown  ();

	/// \brief Reloads textures used by slate renderer
	static void ReloadTextures();

	/// \return The Slate style set for the Shooter game 
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:

	static TSharedRef< class FSlateStyleSet > Create();

private:

	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};

#endif // ATTUNED_ATTUNED_TOOL_STYLE_H
