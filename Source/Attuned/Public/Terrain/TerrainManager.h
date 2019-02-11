/// \file       TerrainManager.h
/// \date       09/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Maxime PERAULT, Vincent STEHLY--CALISTO

#pragma once

/// \brief TODO : Reorder
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/CollisionQueryParams.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "W_InGameUI.h"
#include "Terrain/TerrainSettings.h"
#include "TerrainManager.generated.h"

/// \brief Forward declaration
class AMyCharacter;

/// \brief This class handles all terrain related features
/// \class UTerrainManager
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATTUNED_API UTerrainManager : public UActorComponent
{
public:

	GENERATED_BODY()

public:

	/// \brief Represents a terrain type (Cleaner than strign representations)
	enum ETerrainType
	{
		Rock,	
		Sand,	
		Water,	
		Neutral,
		Corridor
	};

public:

	/// \brief Sets default values for this component's properties
	UTerrainManager();

	/// \brief Called to initialize the component
	void Initialize();

	/// \brief Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// \brief TODO
	void SetInGameGUI				(UW_InGameUI* inGameGUI);
	void SetRockTerrainSettings		(const TerrainSettings& settings);
	void SetSandTerrainSettings		(const TerrainSettings& settings);
	void SetWaterTerrainSettings	(const TerrainSettings&	settings);
	void SetNeutralTerrainSettings	(const TerrainSettings&	settings);
	void SetCorridorTerrainSettings	(const TerrainSettings&	settings);
	void SetRockMomemtumSettings	(const RockMomemtumSettings& settings);
	
	/// \brief TODO
	const TerrainSettings&		GetRockTerrainSettings		() const;
	const TerrainSettings&		GetSandTerrainSettings		() const;
	const TerrainSettings&		GetWaterTerrainSettings		() const;
	const TerrainSettings&		GetNeutralTerrainSettings	() const;
	const TerrainSettings&		GetCorridorTerrainSettings	() const;
	const RockMomemtumSettings&	GetRockMomemtumSettings		() const;

	/// \brief TODO
	ETerrainType				GetTerrainType() const;
	FString						GetTerrainTypeStringRepresentation() const;
	const FVector&				GetTerrainNormal() const;

	/// \brief TODO
	void UpdateDashCoolDown(const bool reset);

public:

	/// \brief TODO
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Terrain, meta = (AllowPrivateAccess = "true"))
	float DashCoolDown;

	/// \brief TODO
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Terrain, meta = (AllowPrivateAccess = "true"))
	bool CanDash;

protected:

	/// \brief Called when the game starts
	virtual void BeginPlay() override;

private:

	AMyCharacter*			m_character;
	UW_InGameUI*			m_inGameUIAttached;

	ETerrainType			m_terrainType;
	FVector					m_terrainNormal;
	FCollisionQueryParams	m_RVTraceParams;
	EPhysicalSurface		m_lastSurfaceType;

	TerrainSettings      m_rockSettings;
	TerrainSettings      m_sandSettings;
	TerrainSettings      m_waterSettings;
	TerrainSettings      m_neutralSettings;
	TerrainSettings		 m_corridorSettings;
	RockMomemtumSettings m_momemtumSettings;

	/// \brief TODO : Sort
	bool    m_lockSpeed;
	bool	m_initialized;
	bool	m_isTransitioningSpeed;
	
	float	m_maxSpeed;
	float	m_deltaTime;
	float	m_elapsedTime;

	FVector m_bufferVelocity;

private:

	/// \brief TODO
	void LockVelocity				(void);

	/// \brief TODO
	void RockTerrainFirstStep		(void);
	void SandTerrainFirstStep		(void);
	void WaterTerrainFirstStep		(void);
	void NeutralTerrainFirstStep	(void);
	void CorridorTerrainFirstStep	(void);
	void UpdateTerrainSettings		(const TerrainSettings& in, TerrainSettings& out);

	/// \brief TODO
	void UpdateCharacterSettings();

	/// \brief TODO
	void CharacterMoveSpeedTransition(const bool InitTransition);
};
