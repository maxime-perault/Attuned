/// \file       STerrainCommonSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/STerrainCommonSettings.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SExpandableArea.h>
#include <SWidgetSwitcher.h>
#include <Widgets/Input/SSpinBox.h>
#include <Widgets/Layout/SScrollBox.h>

#define LOCTEXT_NAMESPACE "STerrainCommonSettings"

void STerrainCommonSettings::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SScrollBox) 
		+ SScrollBox::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SExpandableArea)
				.InitiallyCollapsed(false)
				.HeaderContent()
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("STerrainCommonSettings", "CategoryHeader", "Camera"))
				]
				.BodyContent()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
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
				.InitiallyCollapsed(false)
				.HeaderContent()
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("STerrainCommonSettings", "CategoryHeaders", "Movements"))
				]
				.BodyContent()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
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