/// \file       GAttunedTool.h
/// \date       14/12/2018
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_G_ATTUNED_TOOL_H_
#define ATTUNED_TOOL_G_ATTUNED_TOOL_H_

#include "Engine.h"
#include "EngineUtils.h"
#include "LevelEditor.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

#include "Model/AttunedModel.h"

#include "Attuned/Public/MyCharacter.h"
#include "Attuned/Public/CameraManager.h"
#include "Attuned/Public/TerrainManager.h"

/// \brief Global 
/// \class GAttunedTool
class GAttunedTool
{
public:

	TUniquePtr<AttunedModel>& GetRawModel();

	/// \brief Returns a reference on the internal smart pointer
	static TUniquePtr<GAttunedTool>& Get();
	
	/// \brief Returns the current game instance
	UGameInstance*	GetGameInstance	() const;

	/// \brief  Commits all dirty model caches on the disk
	/// \return True if no error occurs, else false
	bool CommitModel();
	bool CommitProfilePreference();

	/// \brief Marks all caches as dirty
	void InvalidateAllCaches();

	/// \brief  Reverts all dirty model caches with values from the disk
	/// \return True if no error occurs, else false
	bool RevertModel();

	/// \brief  Updates the model depending the current world (Editor or PIE)
	/// \param  data The data to update the model with
	void UpdateModel(CameraData&			data);
	void UpdateModel(CommonDataRock&		data);
	void UpdateModel(CommonDataSand&		data);
	void UpdateModel(CommonDataWater&		data);
	void UpdateModel(CommonDataNeutral&		data);
	void UpdateModel(ProfilePreferenceData& data);
	void UpdateModel(RockMomemtumData&		data);

	/// \brief	Returns the current cached model of the wanted data structure
	/// \return A pointer on the cached model data structure
	///			(may be null if T is not a valide type)
	template<class T> const T* GetModel() const
	{
		return m_attunedModel->GetCache<T>();
	}

	/// \brief Engine callback
	void OnPostWorldCreation		(UWorld* world);
	void OnPostWorldCleanup			(UWorld* world, bool bSessionEnded, bool bCleanupResources);
	void OnPostWorldInitialization	(UWorld* world, const UWorld::InitializationValues values);
	void OnPreWorldFinishDestroy	(UWorld* world);
	void OnPostWorldActorTick		(UWorld* world, ELevelTick tick, float dt);

private:

	friend class FAttunedToolModule;

	/// \brief Initialize the singleton
	static void Initialize();

	/// \brief Destroy the singleton
	static void Destroy   ();

	/// \brief Gets the character actor form the pie world
	void GetGamePointersFromPIEWorld();
	void UpdatePIEValues			();
	void UpdatePIECameraSettings	(const UCameraManager::CameraSettings& settings);

	template<class T>
	void UpdatePIETerrainSettings(const T& settings)
	{
		if (!m_character || !m_pieWorld)
		{
			return;
		}

		m_terrainManager->UpdateTerrainSettings(settings);
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] TerrainSettings Updated."));
	}

private:

	static	TUniquePtr<GAttunedTool>	s_attuned_tool;
			TUniquePtr<AttunedModel>	m_attunedModel;

	// Callback delegates
	FWorldDelegates::FWorldEvent::FDelegate					m_onPostWorldCreationDelegate;
	FWorldDelegates::FWorldEvent::FDelegate					m_onPreWorldFinishDestroyDelegate;
	FWorldDelegates::FWorldCleanupEvent::FDelegate			m_onPostWorldCleanupDelegate;			
	FWorldDelegates::FWorldInitializationEvent::FDelegate	m_onPostWorldInitializationDelegate;
	FWorldDelegates::FOnWorldPostActorTick::FDelegate		m_onPostWorldActorTickDelegate;

	// Called when actors are initialized (usefull to get the main actor)
	UWorld::FOnWorldInitializedActors::FDelegate			m_onWorldInitializedActorsDelegate;

	UWorld* m_editorWorld;
	UWorld* m_pieWorld;

	AMyCharacter*	 m_character;
	UCameraManager*  m_cameraManager;
	UTerrainManager* m_terrainManager;
};

#endif // ATTUNED_TOOL_G_ATTUNED_TOOL_H_