/// \file       STerrainSelector.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/STerrainSelector.h"
#include "Widget/Terrain/SRockTerrainSettings.h"
#include "Widget/Terrain/SSandTerrainSettings.h"
#include "Widget/Terrain/SWaterTerrainSettings.h"
#include "Widget/Terrain/SNeutralTerrainSettings.h"

#include <SBox.h>
#include <SImage.h>
#include <SButton.h>
#include <SWrapBox.h>
#include <STextBlock.h>
#include <SWidgetSwitcher.h>

#include <Framework/MultiBox/MultiBox.h>
#include <Framework/MultiBox/MultiBoxDefs.h>
#include <Framework/MultiBox/MultiBoxBuilder.h>

#include "AttunedToolStyle.h"

#define LOCTEXT_NAMESPACE "STerrainSelector"

void STerrainSelector::Construct(const FArguments& InArgs)
{
	m_tabIndex = 0;

	ChildSlot
	[
		SNew(SBorder)
		.BorderBackgroundColor_Lambda([this](void)->FSlateColor {return GetTerrainColor(); })
		[
			// The widget to switch between terrains
			// Rock - Sand - Water - Neutral
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.MaxHeight(80.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.MaxWidth(80.0f)
				.Padding(FMargin(1.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SButton)
					.ForegroundColor_Lambda([this](void)->FSlateColor {return GetButtonColor(0);  })
					.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor {return GetButtonColor(0);  })
					.OnClicked_Raw(this, &STerrainSelector::OnRockTerrainClick)
					.ToolTipText_Lambda([this](void)->FText {return FText::FromString(TEXT("ToolTip Rock")); })
					[
						TSharedRef<SWidget>(SNew(SImage)
						.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.RockTerrain")))
					]
				]
				+ SHorizontalBox::Slot()
				.MaxWidth(80.0f)
				.Padding(FMargin(1.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SButton)
					.Text(FText::FromString("Sand"))
					.ForegroundColor_Lambda([this](void)->FSlateColor {return GetButtonColor(1);  })
					.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor {return GetButtonColor(1);  })
					.OnClicked_Raw(this, &STerrainSelector::OnSandTerrainClick)
					.ToolTipText_Lambda([this](void)->FText {return FText::FromString(TEXT("ToolTip Sand")); })
					[
						TSharedRef<SWidget>(SNew(SImage)
						.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.SandTerrain")))
					]
				]
				+ SHorizontalBox::Slot()
				.MaxWidth(80.0f)
				.Padding(FMargin(1.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SButton)
					.Text(FText::FromString("Water"))
					.ForegroundColor_Lambda([this](void)->FSlateColor {return GetButtonColor(2);  })
					.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor {return GetButtonColor(2);  })
					.OnClicked_Raw(this, &STerrainSelector::OnWaterTerrainClick)
					.ToolTipText_Lambda([this](void)->FText {return FText::FromString(TEXT("ToolTip Water")); })
					[
						TSharedRef<SWidget>(SNew(SImage)
						.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.WaterTerrain")))
					]
				]
				+ SHorizontalBox::Slot()
				.MaxWidth(80.0f)
				.Padding(FMargin(1.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SButton)
					.Text(FText::FromString("Neutral"))
					.ForegroundColor_Lambda([this](void)->FSlateColor {return GetButtonColor(3);  })
					.ButtonColorAndOpacity_Lambda([this](void)->FSlateColor {return GetButtonColor(3);  })
					.OnClicked_Raw(this, &STerrainSelector::OnNeutralTerrainClick)
					.ToolTipText_Lambda([this](void)->FText {return FText::FromString(TEXT("ToolTip Neutral")); })
					[
						TSharedRef<SWidget>(SNew(SImage)
						.Image(FAttunedToolStyle::Get().GetBrush("AttunedTool.NeutralTerrain")))
					]
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SWidgetSwitcher)
				.WidgetIndex(this, &STerrainSelector::GetCurrentTabIndex)
				+ SWidgetSwitcher::Slot()
				[
					SNew(SRockTerrainSettings)
				]
				+ SWidgetSwitcher::Slot()
				[
					SNew(SSandTerrainSettings)
				]
				+ SWidgetSwitcher::Slot()
				[
					SNew(SWaterTerrainSettings)
				]
				+ SWidgetSwitcher::Slot()
				[
					SNew(SNeutralTerrainSettings)
				]
			]
		]
	];
}

FReply STerrainSelector::OnRockTerrainClick()
{
	m_tabIndex = 0;
	return FReply::Handled();
}

FReply STerrainSelector::OnSandTerrainClick()
{
	m_tabIndex = 1;
	return FReply::Handled();
}

FReply STerrainSelector::OnWaterTerrainClick()
{
	m_tabIndex = 2;
	return FReply::Handled();
}

FReply STerrainSelector::OnNeutralTerrainClick()
{
	m_tabIndex = 3;
	return FReply::Handled();
}

int32 STerrainSelector::GetCurrentTabIndex() const
{
	return m_tabIndex;
}

FLinearColor STerrainSelector::GetTerrainColor() const
{
	switch(m_tabIndex)
	{
		case 0:  return FLinearColor(0.458f, 0.298f, 0.141f, 1.0f); // Rock
		case 1:  return FLinearColor(0.964f, 0.776f, 0.231f, 1.0f); // Sand
		case 2:  return FLinearColor(0.000f, 0.576f, 0.737f, 1.0f); // Water
		case 3:  return FLinearColor(0.400f, 0.400f, 0.400f, 1.0f); // Neutral
		default: break;
	}

	return FLinearColor(0.400f, 0.400f, 0.400f, 1.0f); // Neutral
}

FLinearColor STerrainSelector::GetButtonColor(int32 index) const
{
	if (index == m_tabIndex)
	{
		return FLinearColor(0.243f, 0.243f, 0.243f, 1.0f);
	}

	return FLinearColor(0.086f, 0.086f, 0.086f, 1.0f);
}

#undef LOCTEXT_NAMESPACE