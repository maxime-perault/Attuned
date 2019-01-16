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

TUniquePtr<AttunedModel>& GAttunedTool::GetModel()
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
		}
	}
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
	// TODO : Character update ?

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Model committed."));
	return result;
}

/// \brief  Reverts all dirty model caches with values from the disk
/// \return True if no error occurs, else false
bool GAttunedTool::RevertModel()
{
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Reverting model..."));

	bool result = m_attunedModel->RevertChanges();
	// TODO : Character update ?

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Model reverted."));
	return result;
}

/// \brief  Updates the model depending the current world (Editor or PIE)
/// \param  data The data to update the model with
/// \return True if no error occurs, else false
void GAttunedTool::UpdateModel(CameraData& data)
{
	// TODO : Character update ?
	m_attunedModel->UpdateCache(data);
}

#undef LOCTEXT_NAMESPACE