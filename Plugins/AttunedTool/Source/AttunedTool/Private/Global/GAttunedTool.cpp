/// \file       GAttunedTool.cpp
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Global/GAttunedTool.h"

#define LOCTEXT_NAMESPACE "GAttunedTool"

/* static */ TUniquePtr<GAttunedTool> GAttunedTool::s_attuned_tool;

/* static */ void GAttunedTool::Initialize()
{
	if (!s_attuned_tool)
	{
		s_attuned_tool                 = MakeUnique<GAttunedTool>();
		s_attuned_tool->m_attunedModel = MakeUnique<AttunedModel>();
		s_attuned_tool->m_attunedModel->Initialize();

		s_attuned_tool->m_editorWorld    = nullptr;
		s_attuned_tool->m_pieWorld       = nullptr;
		s_attuned_tool->m_character      = nullptr;
		s_attuned_tool->m_cameraManager  = nullptr;
		s_attuned_tool->m_terrainManager = nullptr;

		// Initializing delegates
		s_attuned_tool->m_onPostWorldCreationDelegate.BindRaw		(s_attuned_tool.Get(), &GAttunedTool::OnPostWorldCreation);
		s_attuned_tool->m_onPreWorldFinishDestroyDelegate.BindRaw	(s_attuned_tool.Get(), &GAttunedTool::OnPreWorldFinishDestroy);
		s_attuned_tool->m_onPostWorldCleanupDelegate.BindRaw		(s_attuned_tool.Get(), &GAttunedTool::OnPostWorldCleanup);
		s_attuned_tool->m_onPostWorldInitializationDelegate.BindRaw	(s_attuned_tool.Get(), &GAttunedTool::OnPostWorldInitialization);
		s_attuned_tool->m_onPostWorldActorTickDelegate.BindRaw		(s_attuned_tool.Get(), &GAttunedTool::OnPostWorldActorTick);
		
		// Binding delegates to engine
		FWorldDelegates::OnPostWorldCleanup.Add		  (s_attuned_tool->m_onPostWorldCleanupDelegate);
		FWorldDelegates::OnPostWorldCreation.Add	  (s_attuned_tool->m_onPostWorldCreationDelegate);
		FWorldDelegates::OnPreWorldFinishDestroy.Add  (s_attuned_tool->m_onPreWorldFinishDestroyDelegate);
		FWorldDelegates::OnPostWorldInitialization.Add(s_attuned_tool->m_onPostWorldInitializationDelegate);
		FWorldDelegates::OnWorldPostActorTick.Add	  (s_attuned_tool->m_onPostWorldActorTickDelegate);

		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Global tool pointer initialized."));
	}
}

/* static */ void GAttunedTool::Destroy()
{
	if (s_attuned_tool)
	{
		s_attuned_tool->m_editorWorld    = nullptr;
		s_attuned_tool->m_pieWorld       = nullptr;
		s_attuned_tool->m_character      = nullptr;
		s_attuned_tool->m_cameraManager  = nullptr;
		s_attuned_tool->m_terrainManager = nullptr;

		// Unbinding engine callbacks
		FWorldDelegates::OnPostWorldCleanup.Remove		 (s_attuned_tool->m_onPostWorldCleanupDelegate.GetHandle());
		FWorldDelegates::OnPostWorldCreation.Remove		 (s_attuned_tool->m_onPostWorldCreationDelegate.GetHandle());
		FWorldDelegates::OnPreWorldFinishDestroy.Remove  (s_attuned_tool->m_onPreWorldFinishDestroyDelegate.GetHandle());
		FWorldDelegates::OnPostWorldInitialization.Remove(s_attuned_tool->m_onPreWorldFinishDestroyDelegate.GetHandle());
		FWorldDelegates::OnWorldPostActorTick.Remove	 (s_attuned_tool->m_onPostWorldActorTickDelegate.GetHandle());

		// Unbinding delegates
		s_attuned_tool->m_onPostWorldCleanupDelegate.Unbind();
		s_attuned_tool->m_onPostWorldCreationDelegate.Unbind();
		s_attuned_tool->m_onPreWorldFinishDestroyDelegate.Unbind();
		s_attuned_tool->m_onPostWorldInitializationDelegate.Unbind();
		s_attuned_tool->m_onPostWorldActorTickDelegate.Unbind();

		s_attuned_tool->m_attunedModel->Destroy();
		s_attuned_tool->m_attunedModel.Reset();

		s_attuned_tool.Reset();
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Global tool pointer destroyed."));
	}
}

/* static */ TUniquePtr<GAttunedTool>& GAttunedTool::Get()
{
	if (!s_attuned_tool)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] The global tool pointer is nullptr."));
	}

	return s_attuned_tool;
}

TUniquePtr<AttunedModel>& GAttunedTool::GetRawModel()
{
	return m_attunedModel;
}

UGameInstance* GAttunedTool::GetGameInstance() const
{
	UGameInstance* p_game_instance = nullptr;

	if (GEngine)
	{
		p_game_instance = ((UGameEngine*)GEngine)->GameInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] The global Unreal Engine pointer is nullptr."));
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Some functionalities may be disabled."));
	}

	return p_game_instance;
}

void GAttunedTool::OnPostWorldInitialization(UWorld* p_world, const UWorld::InitializationValues values)
{
	if (p_world)
	{
		EWorldType::Type worldType = p_world->WorldType;

		if (worldType == EWorldType::Game ||
			worldType == EWorldType::PIE)
		{
			// Updating the PIE world pointer
			m_pieWorld = p_world;
		}
		else if (worldType == EWorldType::Editor)
		{
			// Updating the editor world pointer
			m_editorWorld = p_world;
		}
	}
}

void GAttunedTool::OnPostWorldCreation(UWorld* p_world)
{
	// None
}

void GAttunedTool::OnPostWorldCleanup(UWorld* p_world, bool bSessionEnded, bool bCleanupResources)
{
	if (p_world)
	{
		EWorldType::Type worldType = p_world->WorldType;

		if (worldType == EWorldType::Game ||
			worldType == EWorldType::PIE)
		{
			m_pieWorld       = nullptr;
			m_character      = nullptr;
			m_cameraManager  = nullptr;
			m_terrainManager = nullptr;
		}
		else if (worldType == EWorldType::Editor)
		{
			m_editorWorld = nullptr;
		}
	}
}

void GAttunedTool::OnPreWorldFinishDestroy(UWorld* p_world)
{
	// None
}

void GAttunedTool::GetGamePointersFromPIEWorld()
{
	TActorIterator<AMyCharacter> ActorItr(m_pieWorld);
	for (; ActorItr; ++ActorItr)
	{
		AMyCharacter* p_character = *ActorItr;
		if (p_character)
		{
			m_character      = p_character;
			m_cameraManager  = m_character->mc_CameraManager;
			m_terrainManager = m_character->mc_TerrainManager;

			if (!m_cameraManager || m_terrainManager)
			{
				UE_LOG(LogTemp, Warning, TEXT("[Attuned] The camera and/or the terrain manager are nullptr."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[Attuned] Character found.."));
			}
			
			UpdatePIEValues();
		}
	}
}

void GAttunedTool::UpdatePIEValues()
{
	if (!m_character || !m_pieWorld)
	{
		return;
	}

	// Gets all cached models
	const auto* cameraData        = GetModel<CameraData>();
	const auto* commonDataRock    = GetModel<CommonDataRock>();
	const auto* commonDataSand    = GetModel<CommonDataSand>();
	const auto* commonDataWater   = GetModel<CommonDataWater>();
	const auto* commonDataNeutral = GetModel<CommonDataNeutral>();

	UCameraManager::CameraSettings cameraSettings {};
	cameraSettings.MaxArmLength         = cameraData->m_maxArmLenghtValue;
	cameraSettings.MaxTimeFromLastInput = cameraData->m_maxTimeFromLastInputValue;

	// Updating camera
	m_cameraManager->UpdateCameraSettings(cameraSettings);

	// Updating terrains
	UTerrainManager::RockTerrainSettings rockSettings {};
	rockSettings.RockSpeed				   = commonDataRock->m_maxSpeedValue;
	rockSettings.RockAirControl			   = commonDataRock->m_airControlValue;
	rockSettings.RockAcceleration		   = commonDataRock->m_accelerationValue;
	rockSettings.RockJumpZVelocity		   = commonDataRock->m_jumpZVelocityValue;
	rockSettings.RockFallingFriction	   = commonDataRock->m_fallingFrictionValue;

	UTerrainManager::SandTerrainSettings sandSettings {};
	sandSettings.SandSpeed				   = commonDataSand->m_maxSpeedValue;
	sandSettings.SandAirControl			   = commonDataSand->m_airControlValue;
	sandSettings.SandAcceleration		   = commonDataSand->m_accelerationValue;
	sandSettings.SandJumpZVelocity		   = commonDataSand->m_jumpZVelocityValue;
	sandSettings.SandFallingFriction	   = commonDataSand->m_fallingFrictionValue;

	UTerrainManager::WaterTerrainSettings waterSettings {};
	waterSettings.WaterSpeed			   = commonDataWater->m_maxSpeedValue;
	waterSettings.WaterAirControl		   = commonDataWater->m_airControlValue;
	waterSettings.WaterAcceleration		   = commonDataWater->m_accelerationValue;
	waterSettings.WaterAcceleration		   = commonDataWater->m_jumpZVelocityValue;
	waterSettings.WaterFallingFriction	   = commonDataWater->m_fallingFrictionValue;

	UTerrainManager::NeutralTerrainSettings neutralSettings {};
	neutralSettings.DefaultSpeed           = commonDataNeutral->m_maxSpeedValue;
	neutralSettings.DefaultAirControl      = commonDataNeutral->m_airControlValue;
	neutralSettings.DefaultAcceleration    = commonDataNeutral->m_accelerationValue;
	neutralSettings.DefaultJumpZVelocity   = commonDataNeutral->m_jumpZVelocityValue;
	neutralSettings.DefaultFallingFriction = commonDataNeutral->m_fallingFrictionValue;

	m_terrainManager->UpdateTerrainSettings(rockSettings);
	m_terrainManager->UpdateTerrainSettings(sandSettings);
	m_terrainManager->UpdateTerrainSettings(waterSettings);
	m_terrainManager->UpdateTerrainSettings(neutralSettings);
}

void GAttunedTool::UpdatePIECameraSettings(const UCameraManager::CameraSettings& settings)
{
	if (!m_character || !m_pieWorld)
	{
		return;
	}

	m_cameraManager->UpdateCameraSettings(settings);
}

void GAttunedTool::OnPostWorldActorTick(UWorld* world, ELevelTick tick, float dt)
{
	if (m_pieWorld && !m_character)
	{
		GetGamePointersFromPIEWorld();
	}
}

/// \brief  Commits all dirty model caches on the disk
/// \return True if no error occurs, else false
bool GAttunedTool::CommitModel()
{
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Committing model..."));

	bool result = m_attunedModel->CommitChanges();

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Model committed."));
	return result;
}

bool GAttunedTool::CommitProfilePreference()
{
	return m_attunedModel->CommitProfilePreference();
}

/// \brief Marks all caches as dirty
void GAttunedTool::InvalidateAllCaches()
{
	m_attunedModel->InvalidateAllCaches();
}

/// \brief  Reverts all dirty model caches with values from the disk
/// \return True if no error occurs, else false
bool GAttunedTool::RevertModel()
{
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Reverting model..."));

	bool result = m_attunedModel->RevertChanges();
	
	// Applying modifications on the character
	UpdatePIEValues();

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Model reverted."));
	return result;
}

/// \brief  Updates the model depending the current world (Editor or PIE)
/// \param  data The data to update the model with
/// \return True if no error occurs, else false
void GAttunedTool::UpdateModel(CameraData& data)
{
	m_attunedModel->UpdateCache(data);

	UCameraManager::CameraSettings cameraSettings{};
	cameraSettings.MaxArmLength         = data.m_maxArmLenghtValue;
	cameraSettings.MaxTimeFromLastInput = data.m_maxTimeFromLastInputValue;

	UpdatePIECameraSettings(cameraSettings);
}

void GAttunedTool::UpdateModel(CommonDataRock& data)
{
	m_attunedModel->UpdateCache(data);

	UTerrainManager::RockTerrainSettings rockSettings{};
	rockSettings.RockSpeed           = data.m_maxSpeedValue;
	rockSettings.RockAirControl      = data.m_airControlValue;
	rockSettings.RockAcceleration    = data.m_accelerationValue;
	rockSettings.RockJumpZVelocity   = data.m_jumpZVelocityValue;
	rockSettings.RockFallingFriction = data.m_fallingFrictionValue;

	UpdatePIETerrainSettings(rockSettings);
}

void GAttunedTool::UpdateModel(CommonDataSand& data)
{
	m_attunedModel->UpdateCache(data);

	UTerrainManager::SandTerrainSettings sandSettings{};
	sandSettings.SandSpeed           = data.m_maxSpeedValue;
	sandSettings.SandAirControl      = data.m_airControlValue;
	sandSettings.SandAcceleration    = data.m_accelerationValue;
	sandSettings.SandJumpZVelocity   = data.m_jumpZVelocityValue;
	sandSettings.SandFallingFriction = data.m_fallingFrictionValue;

	UpdatePIETerrainSettings(sandSettings);
}

void GAttunedTool::UpdateModel(CommonDataWater&	data)
{
	m_attunedModel->UpdateCache(data);

	UTerrainManager::WaterTerrainSettings waterSettings{};
	waterSettings.WaterSpeed           = data.m_maxSpeedValue;
	waterSettings.WaterAirControl      = data.m_airControlValue;
	waterSettings.WaterAcceleration    = data.m_accelerationValue;
	waterSettings.WaterAcceleration    = data.m_jumpZVelocityValue;
	waterSettings.WaterFallingFriction = data.m_fallingFrictionValue;

	UpdatePIETerrainSettings(waterSettings);
}

void GAttunedTool::UpdateModel(CommonDataNeutral& data)
{
	m_attunedModel->UpdateCache(data);

	UTerrainManager::NeutralTerrainSettings neutralSettings{};
	neutralSettings.DefaultSpeed           = data.m_maxSpeedValue;
	neutralSettings.DefaultAirControl      = data.m_airControlValue;
	neutralSettings.DefaultAcceleration    = data.m_accelerationValue;
	neutralSettings.DefaultJumpZVelocity   = data.m_jumpZVelocityValue;
	neutralSettings.DefaultFallingFriction = data.m_fallingFrictionValue;

	UpdatePIETerrainSettings(neutralSettings);
}

void GAttunedTool::UpdateModel(ProfilePreferenceData& data)
{
	m_attunedModel->UpdateCache(data);
}

#undef LOCTEXT_NAMESPACE