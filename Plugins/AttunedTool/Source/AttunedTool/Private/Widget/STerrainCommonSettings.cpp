/// \file       STerrainCommonSettings.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Widget/STerrainCommonSettings.h"

#include "Engine.h"
#include "Editor.h"

#include <SButton.h>
#include <STextBlock.h>
#include <SExpandableArea.h>
#include <SWidgetSwitcher.h>
#include <Widgets/Input/SSpinBox.h>
#include <Widgets/Layout/SScrollBox.h>

#include "Global/GAttunedTool.h"

#define LOCTEXT_NAMESPACE "STerrainCommonSettings"

void STerrainCommonSettings::Construct(const FArguments& InArgs)
{
	m_cameraMaxArmLenght         = InArgs._cameraMaxArmLenght;
	m_cameraMaxTimeFromLastInput = InArgs._cameraMaxTimeFromLastInput;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.BorderBackgroundColor_Lambda    ([this](void)->FSlateColor { return GetExpandableBorderColor(); })
			.BodyBorderBackgroundColor_Lambda([this](void)->FSlateColor { return GetExpandableBodyColor  (); })
			.HeaderContent()
			[
				SNew(STextBlock)
				.Text(NSLOCTEXT("STerrainCommonSettings", "CategoryHeader", "Camera"))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 10))
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
						.Text(NSLOCTEXT("STerrainCommonSettings", "Common1", "Max Arm Lenght"))
					]
					+ SHorizontalBox::Slot()
					[
						m_cameraMaxArmLenght.ToSharedRef()
					]
				]
				+ SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 10.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[				
						SNew(STextBlock)
						.Text(NSLOCTEXT("STerrainCommonSettings", "Common2", "Max Time From Last Input"))
					]
					+ SHorizontalBox::Slot()
					[
						m_cameraMaxTimeFromLastInput.ToSharedRef()
					]
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.BorderBackgroundColor_Lambda    ([this](void)->FSlateColor { return GetExpandableBorderColor(); })
			.BodyBorderBackgroundColor_Lambda([this](void)->FSlateColor { return GetExpandableBodyColor();   })
			.InitiallyCollapsed(true)
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
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("STerrainCommonSettings", "Common3", "Falling Friction"))
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSpinBox<float>)
						.Value_Lambda         ([this](void)->float   { return m_fallingFrictionValue;     })
						.MinValue_Lambda      ([this](void)->float   { return m_fallingFrictionMinValue;  })
						.MaxValue_Lambda      ([this](void)->float   { return m_fallingFrictionMaxValue;  })
						.OnValueChanged_Lambda([this](float v)->void { m_fallingFrictionValue = v;        })
						.ToolTipText_Lambda   ([this](void)->FText   { return FText::FromString("TODO");  })
					]
				]
				+ SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("STerrainCommonSettings", "Common4", "Air Control"))
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSpinBox<float>)
						.Value_Lambda         ([this](void)->float   { return m_airControlValue;         })
						.MinValue_Lambda      ([this](void)->float   { return m_airControlMinValue;      })
						.MaxValue_Lambda      ([this](void)->float   { return m_airControlMaxValue;      })
						.OnValueChanged_Lambda([this](float v)->void { m_airControlValue = v;            })
						.ToolTipText_Lambda   ([this](void)->FText   { return FText::FromString("TODO"); })
					]
				]
				+ SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("STerrainCommonSettings", "Common5", "Jump Z velocity"))
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSpinBox<float>)
						.Value_Lambda         ([this](void)->float   { return m_jumpZVelocityValue;      })
						.MinValue_Lambda      ([this](void)->float   { return m_jumpZVelocityMinValue;   })
						.MaxValue_Lambda      ([this](void)->float   { return m_jumpZVelocityMaxValue;   })
						.OnValueChanged_Lambda([this](float v)->void { m_jumpZVelocityValue = v;         })
						.ToolTipText_Lambda   ([this](void)->FText   { return FText::FromString("TODO"); })
					]
				]
				+ SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("STerrainCommonSettings", "Common6", "Dash Cooldown"))
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSpinBox<float>)
						.Value_Lambda         ([this](void)->float   { return m_dashCooldownValue;        })
						.MinValue_Lambda      ([this](void)->float   { return m_dashCooldownMinValue;     })
						.MaxValue_Lambda      ([this](void)->float   { return m_dashCooldownMaxValue;     })
						.OnValueChanged_Lambda([this](float v)->void { m_dashCooldownValue = v;           })
						.ToolTipText_Lambda   ([this](void)->FText   { return FText::FromString("TODO");  })
					]
				]
				+ SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("STerrainCommonSettings", "Common7", "Acceleration"))
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSpinBox<float>)
						.Value_Lambda         ([this](void)->float   { return m_accelerationValue;       })
						.MinValue_Lambda      ([this](void)->float   { return m_accelerationMinValue;    })
						.MaxValue_Lambda      ([this](void)->float   { return m_accelerationMaxValue;    })
						.OnValueChanged_Lambda([this](float v)->void { m_accelerationValue = v;          })
						.ToolTipText_Lambda   ([this](void)->FText   { return FText::FromString("TODO"); })
					]
				]
				+ SVerticalBox::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("STerrainCommonSettings", "Common8", "Max speed"))
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSpinBox<float>)
						.Value_Lambda         ([this](void)->float   { return m_maxSpeedValue;           })
						.MinValue_Lambda      ([this](void)->float   { return m_maxSpeedMinValue;        })
						.MaxValue_Lambda      ([this](void)->float   { return m_maxSpeedMaxValue;        })
						.OnValueChanged_Lambda([this](float v)->void { m_maxSpeedValue = v;              })
						.ToolTipText_Lambda   ([this](void)->FText   { return FText::FromString("TODO"); })
					]
				]
			]
		]
	];
}

/* virtual */ void STerrainCommonSettings::Tick(
	const FGeometry& AllottedGeometry, 
	const double InCurrentTime, 
	const float InDeltaTime)
{
	// TODO
}

/* virtual */ void STerrainCommonSettings::ApplyChanges()
{
	/*CameraData data;
	data.m_maxArmLenghtValue         = test.Get()->GetValue();
	data.m_maxTimeFromLastInputValue = test.Get()->GetValue();

	GAttunedTool::Get()->GetModel()->SerializeData<CameraData>(data);

	UE_LOG(LogTemp, Log, TEXT("[Attuned] Value found : %lf"), test.Get()->GetValue());*/
}

/* virtual */ void STerrainCommonSettings::ResetChanges()
{
	/*CameraData data;
	GAttunedTool::Get()->GetModel()->DeserializeData<CameraData>(data);

	m_maxArmLenghtValue         = data.m_maxArmLenghtValue;
	m_maxTimeFromLastInputValue = data.m_maxTimeFromLastInputValue;

	test.Get()->SetValue(data.m_maxArmLenghtValue);

	UE_LOG(LogTemp, Log, TEXT("[Attuned] Value found : %lf"), data.m_maxArmLenghtValue);*/
}

FSlateColor STerrainCommonSettings::GetExpandableBodyColor  () const
{
	return FSlateColor(FLinearColor(1.00f, 0.00f, 0.00f, 1.00f));
}

FSlateColor STerrainCommonSettings::GetExpandableBorderColor() const
{
	return FSlateColor(FLinearColor(0.18f, 0.18f, 0.18f, 1.00f));
}

#undef LOCTEXT_NAMESPACE