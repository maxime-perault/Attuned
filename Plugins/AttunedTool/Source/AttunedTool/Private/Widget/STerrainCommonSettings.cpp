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
	m_terrainType				 = InArgs._terrainType;
	m_cameraMaxArmLenght         = InArgs._cameraMaxArmLenght;
	m_cameraMaxTimeFromLastInput = InArgs._cameraMaxTimeFromLastInput;

	m_fallingFriction = SNew(SSpinBox<float>)
		.MinValue(CommonData::FallingFrictionMinValue)
		.MaxValue(CommonData::FallingFrictionMaxValue)
		.Value_Raw		(this, &STerrainCommonSettings::FallingFrictionValue)
		.OnValueChanged	(this, &STerrainCommonSettings::OnFallingFrictionValueChanged);

	m_airControl = SNew(SSpinBox<float>)
		.MinValue(CommonData::AirControlMinValue)
		.MaxValue(CommonData::AirControlMaxValue)
		.Value_Raw		(this, &STerrainCommonSettings::AirControlValue)
		.OnValueChanged	(this, &STerrainCommonSettings::OnAirControlValueChanged);

	m_jumpZVelocity = SNew(SSpinBox<float>)
		.MinValue(CommonData::JumpZVelocityMinValue)
		.MaxValue(CommonData::JumpZVelocityMaxValue)
		.Value_Raw		(this, &STerrainCommonSettings::JumpZVelocityValue)
		.OnValueChanged	(this, &STerrainCommonSettings::OnJumpZVelocityValueChanged);

	m_dashCooldown = SNew(SSpinBox<float>)
		.MinValue(CommonData::DashCooldownMinValue)
		.MaxValue(CommonData::DashCooldownMaxValue)
		.Value_Raw		(this, &STerrainCommonSettings::DashCooldownValue)
		.OnValueChanged	(this, &STerrainCommonSettings::OnDashCooldownValueChanged);

	m_acceleration = SNew(SSpinBox<float>)
		.MinValue(CommonData::AccelerationMinValue)
		.MaxValue(CommonData::AccelerationMaxValue)
		.Value_Raw		(this, &STerrainCommonSettings::AccelerationValue)
		.OnValueChanged	(this, &STerrainCommonSettings::OnAccelerationValueChanged);

	m_maxSpeed = SNew(SSpinBox<float>)
		.MinValue(CommonData::MaxSpeedMinValue)
		.MaxValue(CommonData::MaxSpeedMaxValue)
		.Value_Raw		(this, &STerrainCommonSettings::MaxSpeedValue)
		.OnValueChanged	(this, &STerrainCommonSettings::OnMaxSpeedValueChanged);

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
						m_fallingFriction.ToSharedRef()
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
						m_airControl.ToSharedRef()
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
						m_jumpZVelocity.ToSharedRef()
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
						m_dashCooldown.ToSharedRef()
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
						m_acceleration.ToSharedRef()
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
						m_maxSpeed.ToSharedRef()
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
	// None
}

/* virtual */ void STerrainCommonSettings::ApplyChanges()
{
	// None
}

/* virtual */ void STerrainCommonSettings::ResetChanges()
{
	// None
}

FSlateColor STerrainCommonSettings::GetExpandableBodyColor  () const
{
	return FSlateColor(FLinearColor(1.00f, 0.00f, 0.00f, 1.00f));
}

FSlateColor STerrainCommonSettings::GetExpandableBorderColor() const
{
	return FSlateColor(FLinearColor(0.18f, 0.18f, 0.18f, 1.00f));
}

void STerrainCommonSettings::OnFallingFrictionValueChanged(float value)
{
	CommonDataGeneric data;
	BuildCommonDataFromWidget(data);
	data.m_fallingFrictionValue = value;

	UpdateModelFromCommonData(data);
}

void STerrainCommonSettings::OnAirControlValueChanged(float value)
{
	CommonDataGeneric data;
	BuildCommonDataFromWidget(data);
	data.m_airControlValue = value;

	UpdateModelFromCommonData(data);
}

void STerrainCommonSettings::OnJumpZVelocityValueChanged(float value)
{
	CommonDataGeneric data;
	BuildCommonDataFromWidget(data);
	data.m_jumpZVelocityValue = value;

	UpdateModelFromCommonData(data);
}

void STerrainCommonSettings::OnDashCooldownValueChanged(float value)
{
	CommonDataGeneric data;
	BuildCommonDataFromWidget(data);
	data.m_dashCooldownValue = value;

	UpdateModelFromCommonData(data);
}

void STerrainCommonSettings::OnAccelerationValueChanged(float value)
{
	CommonDataGeneric data;
	BuildCommonDataFromWidget(data);
	data.m_accelerationValue = value;

	UpdateModelFromCommonData(data);
}

void STerrainCommonSettings::OnMaxSpeedValueChanged(float value)
{
	CommonDataGeneric data;
	BuildCommonDataFromWidget(data);
	data.m_maxSpeedValue = value;

	UpdateModelFromCommonData(data);
}

float STerrainCommonSettings::FallingFrictionValue() const
{
	const CommonData* cache = GetCommonDataCache();
	return cache->m_fallingFrictionValue;
}

float STerrainCommonSettings::AirControlValue() const
{
	const CommonData* cache = GetCommonDataCache();
	return cache->m_airControlValue;
}

float STerrainCommonSettings::JumpZVelocityValue() const
{
	const CommonData* cache = GetCommonDataCache();
	return cache->m_jumpZVelocityValue;
}

float STerrainCommonSettings::DashCooldownValue() const
{
	const CommonData* cache = GetCommonDataCache();
	return cache->m_dashCooldownValue;
}

float STerrainCommonSettings::AccelerationValue() const
{
	const CommonData* cache = GetCommonDataCache();
	return cache->m_accelerationValue;
}

float STerrainCommonSettings::MaxSpeedValue() const
{
	const CommonData* cache = GetCommonDataCache();
	return cache->m_maxSpeedValue;
}

const CommonData* STerrainCommonSettings::GetCommonDataCache() const
{
	switch (m_terrainType)
	{
		case ETerrainType::Rock:    { return GAttunedTool::Get()->GetModel<CommonDataRock>();    break; }
		case ETerrainType::Sand:    { return GAttunedTool::Get()->GetModel<CommonDataSand>();    break; }
		case ETerrainType::Water:   { return GAttunedTool::Get()->GetModel<CommonDataWater>();   break; }
		case ETerrainType::Neutral: { return GAttunedTool::Get()->GetModel<CommonDataNeutral>(); break; }
	}

	return nullptr;
}

void STerrainCommonSettings::BuildCommonDataFromWidget(CommonDataGeneric& data) const
{
	data.m_fallingFrictionValue = m_fallingFriction->GetValue();
	data.m_airControlValue	    = m_airControl->GetValue();
	data.m_jumpZVelocityValue   = m_jumpZVelocity->GetValue();
	data.m_dashCooldownValue    = m_dashCooldown->GetValue();
	data.m_accelerationValue    = m_acceleration->GetValue();
	data.m_maxSpeedValue        = m_maxSpeed->GetValue();
}

void STerrainCommonSettings::UpdateModelFromCommonData(const CommonDataGeneric& data) const
{
	switch (m_terrainType)
	{
		case ETerrainType::Rock: 
		{
			CommonDataRock rockData;
			CommonData::Copy(data, rockData);

			GAttunedTool::Get()->UpdateModel(rockData);
			break;
		}

		case ETerrainType::Sand: 
		{
			CommonDataSand sandData;
			CommonData::Copy(data, sandData);

			GAttunedTool::Get()->UpdateModel(sandData);
			break;
		}

		case ETerrainType::Water: 
		{
			CommonDataWater waterData;
			CommonData::Copy(data, waterData);

			GAttunedTool::Get()->UpdateModel(waterData);
			break;
		}

		case ETerrainType::Neutral: 
		{
			CommonDataNeutral neutralData;
			CommonData::Copy(data, neutralData);

			GAttunedTool::Get()->UpdateModel(neutralData);
			break;
		}
	}
}

#undef LOCTEXT_NAMESPACE