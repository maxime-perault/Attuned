/// \file       STerrainCommonSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/STerrainCommonSettings.h"

#include "Engine.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SExpandableArea.h>
#include <SWidgetSwitcher.h>
#include <Widgets/Input/SSpinBox.h>
#include <Widgets/Layout/SScrollBox.h>

#define LOCTEXT_NAMESPACE "STerrainCommonSettings"

void STerrainCommonSettings::Construct(const FArguments& InArgs)
{
	// .Font_Lambda([this](void)->FSlateFontInfo {return FSlateFontInfo(TEXT("CompositeVerdana"), 8, EFontHinting::Auto, FFontOutlineSettings(1, FLinearColor(0.878f, 0.878f, 0.878f, 1.0f))); })

	   // Font'/Game/Fonts/CompositeVerdana.CompositeVerdana'
	UObject* obj_ptr = StaticLoadObject(UFont::StaticClass(), NULL, TEXT("/Engine/EngineFonts/Roboto"));
	UFont*  font_ptr = Cast<UFont>(obj_ptr);

	if (!font_ptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULLFONT"));
	}

	ChildSlot //0.243
	[
		SNew(SScrollBox) 
		+ SScrollBox::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SExpandableArea)
				.BorderBackgroundColor_Lambda    ([this](void)->FSlateColor {return FSlateColor(FLinearColor(0.180f, 0.180f, 0.180f, 1.0f));  })
				.BodyBorderBackgroundColor_Lambda([this](void)->FSlateColor {return FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));  })
				.InitiallyCollapsed(false)
				.HeaderContent()
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("STerrainCommonSettings", "CategoryHeader", "Camera"))
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 10))
					/*.Font_Lambda([this](void)->FSlateFontInfo 
					{
						return FSlateFontInfo(StaticLoadObject(UFont::StaticClass(), NULL, TEXT("/Engine/EngineFonts/Roboto")),
							8, TEXT("Bolds")); })*/
		

					//.HighlightColor_Lambda([this](void)->FLinearColor {return FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);  })
				]
				.BodyContent()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO1", "Max Arm Lenght"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_maxArmLenght; })
							.MaxValue_Lambda([this](void)->float { return   1000.0f; })
							.MinValue_Lambda([this](void)->float { return   10.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_maxArmLenght = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
					+ SVerticalBox::Slot()
					.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[				
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO2", "Max Time From Last Input"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_maxTimeFromLastInput; })
							.MaxValue_Lambda([this](void)->float { return   1000.0f; })
							.MinValue_Lambda([this](void)->float { return   10.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_maxTimeFromLastInput = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
				]
			]
		]
		+ SScrollBox::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SExpandableArea)
				.BorderBackgroundColor_Lambda([this](void)->FSlateColor {return FSlateColor(FLinearColor(0.180f, 0.180f, 0.180f, 1.0f));  })
				.BodyBorderBackgroundColor_Lambda([this](void)->FSlateColor {return FSlateColor(FLinearColor(0.3f, 0.0f, 0.0f, 1.0f));  })
				.InitiallyCollapsed(false)
				.HeaderContent()
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("STerrainCommonSettings", "CategoryHeaders", "Movements"))
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 10))
				]
				.BodyContent()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(FMargin(10.0f, 10.0f, 0.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
	
						//.Padding(FMargin(10.0f, 0.0f, 0.0f, 0.0f))
						[
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO3", "Falling Friction"))
							
						]
						+ SHorizontalBox::Slot() // m_airControl
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_fallingFriction; })
							.MaxValue_Lambda([this](void)->float { return   1000.0f; })
							.MinValue_Lambda([this](void)->float { return   10.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_fallingFriction = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
					+ SVerticalBox::Slot() // m_jumpZVelocity
				    .Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO4", "Air Control"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_airControl; })
							.MaxValue_Lambda([this](void)->float { return   1000.0f; })
							.MinValue_Lambda([this](void)->float { return   10.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_airControl = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
					+ SVerticalBox::Slot() // m_dashCooldown
					.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO5", "Jump Z velocity"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_jumpZVelocity; })
							.MaxValue_Lambda([this](void)->float { return   1000.0f; })
							.MinValue_Lambda([this](void)->float { return   10.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_jumpZVelocity = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
					+ SVerticalBox::Slot() // m_dashCooldown
					.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO6", "Dash Cooldown"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_dashCooldown; })
							.MaxValue_Lambda([this](void)->float { return   10.0f; })
							.MinValue_Lambda([this](void)->float { return   0.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_dashCooldown = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
					+ SVerticalBox::Slot() // m_dashCooldown
					.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO7", "Acceleration"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_acceleration; })
							.MaxValue_Lambda([this](void)->float { return   10.0; })
							.MinValue_Lambda([this](void)->float { return   1.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_acceleration = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
					+ SVerticalBox::Slot()
					.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO8", "Min speed"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_minSpeed; })
							.MaxValue_Lambda([this](void)->float { return   100.0;   })
							.MinValue_Lambda([this](void)->float { return   1000.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_minSpeed = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
					+ SVerticalBox::Slot()
					.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(NSLOCTEXT("STerrainCommonSettings", "TODO9", "Max speed"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SSpinBox<float>)
							.Value_Lambda([this](void)->float { return   m_maxSpeed; })
							.MaxValue_Lambda([this](void)->float { return   100.0;   })
							.MinValue_Lambda([this](void)->float { return   1000.0f; })
							.OnValueChanged_Lambda([this](float v)->void { m_maxSpeed = v;      })
							.ToolTipText_Lambda([this](void)->FText { return  FText::FromString("TODO"); })
						]
					]
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE