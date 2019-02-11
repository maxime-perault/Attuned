/// \file       TerrainManager.cpp
/// \date       09/02/2018
/// \project    Attuned
/// \package    Attuned
/// \author     Maxime PERAULT, Vincent STEHLY--CALISTO

#include "Terrain/TerrainManager.h"
#include "MyCharacter.h"
#include "Public/CollisionQueryParams.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/GameEngine.h"
#include "Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

/// \brief Sets default values for this component's properties
UTerrainManager::UTerrainManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_lastSurfaceType = SurfaceType_Default;
	m_terrainType     = ETerrainType::Neutral;

	// This component is not yet initialized
	m_initialized = false;
}

/// \brief Called to initialize the component
void UTerrainManager::Initialize()
{
	m_character   = StaticCast<AMyCharacter*>(GetOwner());
	m_initialized = true;
}

/// \brief Called when the game starts
void UTerrainManager::BeginPlay()
{
	Super::BeginPlay();

	m_RVTraceParams = FCollisionQueryParams(true);
	m_RVTraceParams.bTraceComplex           = true;
	m_RVTraceParams.bTraceAsyncScene        = true;
	m_RVTraceParams.bReturnPhysicalMaterial = true;

	m_rockSettings.FallingFriction     = 0.5f;
	m_rockSettings.AirControl          = 0.5f;
	m_rockSettings.JumpZVelocity       = 300.0f;
	m_rockSettings.Acceleration        = 175.0f;
	m_rockSettings.Speed               = 1425.0f;
								       
	m_sandSettings.FallingFriction     = 10.0f;
	m_sandSettings.AirControl	       = 0.2f;
	m_sandSettings.JumpZVelocity       = 1400.0f;
	m_sandSettings.Acceleration        = 925.0f;
	m_sandSettings.Speed               = 950.0f;

	m_waterSettings.FallingFriction    = 1.0f;
	m_waterSettings.AirControl	       = 1.0f;
	m_waterSettings.JumpZVelocity      = 400.0f;
	m_waterSettings.Acceleration       = 500.0f;
	m_waterSettings.Speed              = 1425.0f;

	m_neutralSettings.FallingFriction  = 0.5f;
	m_neutralSettings.AirControl	   = 0.5f;
	m_neutralSettings.JumpZVelocity    = 700.0f;
	m_neutralSettings.Acceleration     = 850.0f;
	m_neutralSettings.Speed            = 825.0f;

	m_corridorSettings.FallingFriction = 0.5f;
	m_corridorSettings.AirControl	   = 0.5f;
	m_corridorSettings.JumpZVelocity   = 700.0f;
	m_corridorSettings.Acceleration    = 850.0f;
	m_corridorSettings.Speed           = 825.0f;

	m_momemtumSettings.IsActive = true;
	m_momemtumSettings.IsSquare = false;
	m_momemtumSettings.MinValue = 0.25f;

	CanDash       = true;
	DashCoolDown  = 0.0f;

	m_lockSpeed = false;
	m_maxSpeed  = 825.f;

	m_elapsedTime          = 0.0f;
	m_isTransitioningSpeed = false;
}

/// \brief Called every frame
/* virtrual */ void UTerrainManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_deltaTime = DeltaTime;

	if (!m_initialized)
	{
		// The component is not yet initialized
		return;
	}
	
	if (!CanDash)
	{
		// The player is in CD
		UpdateDashCoolDown(false);
	}
		
	if (m_isTransitioningSpeed)
	{
		// Lerping velocity between terrains
		CharacterMoveSpeedTransition(false);
	}
		
	// Checking the terrain under the player feet
	FHitResult RVHit(ForceInit);
	FVector    location = m_character->GetActorLocation();

	// Raycast from the middle of player with a (0.0f, 0.0f, 150.0f) vector pointing down
	bool success = GetWorld()->LineTraceSingleByChannel(
		RVHit,													// Result
		FVector(location),										// Start
		FVector(location.X, location.Y, location.Z - 150.0f),	// End
		ECC_Visibility,											// Collision channel
		m_RVTraceParams										    // Params
	);

	if (!success)
	{
		// Not on the ground
		return;
	}
		
	m_terrainNormal                 = RVHit.ImpactNormal;
	EPhysicalSurface CurrentSurface = RVHit.PhysMaterial.Get()->SurfaceType;

	if ((m_lastSurfaceType != CurrentSurface)
      && CurrentSurface    != SurfaceType4)   // DestructibleMeshSurface
	{
		m_lastSurfaceType = CurrentSurface;

		switch (CurrentSurface)
		{
			case SurfaceType_Default: { NeutralTerrainFirstStep();	break; }
			case SurfaceType1:		  { RockTerrainFirstStep();		break; }
			case SurfaceType2:		  { WaterTerrainFirstStep();	break; }
			case SurfaceType3:		  { SandTerrainFirstStep();		break; }
			case SurfaceType8:		  { CorridorTerrainFirstStep();	break; }
			default:
			{
				NeutralTerrainFirstStep();
				break;
			}
		}
	}

	LockVelocity();
}

/// \brief When the player enters the water terrain, his velocity
///		   is kept durint 0.3 seconds in order to avoid a brutal deceleration
void UTerrainManager::LockVelocity(void)
{
	if (!m_lockSpeed)
	{
		return;
	}

	m_elapsedTime += m_deltaTime;
	m_character->GetCharacterMovement()->Velocity = m_bufferVelocity; 

	if (m_elapsedTime >= 0.3f)
	{
		m_elapsedTime = 0.0f;
		m_lockSpeed   = false;

		UE_LOG(LogTemp, Warning, TEXT("Velocity Unlokced"));
	}
}

void UTerrainManager::UpdateDashCoolDown(const bool reset)
{
	static float		s_currentTime = 0.f;
	static const float	s_dashCoolDown = 1.f;

	if (reset)
	{
		s_currentTime = 0.f;
		CanDash       = false;
		m_inGameUIAttached->SetCoolDown(0.f);

		return;
	}

	s_currentTime += m_deltaTime;
	if (s_currentTime > s_dashCoolDown)
	{
		s_currentTime = s_dashCoolDown;
	}
		
	DashCoolDown = s_currentTime / s_dashCoolDown;

	// Updating the GUI ?
	// m_inGameUIAttached->SetCoolDown(DashCoolDown);

	if (s_currentTime == s_dashCoolDown)
	{
		CanDash = true;
	}	
}

void UTerrainManager::CharacterMoveSpeedTransition(const bool InitTransition)
{
	static float		s_CurrentTime         = 0.0f;
	static float		s_StartingMaxSpeed    = 0.0f;
	static const float	s_SpeedTransitionTime = 1.5f;

	if (InitTransition)
	{
		s_CurrentTime          = 0.0f;
		s_StartingMaxSpeed     = m_character->GetCharacterMovement()->Velocity.Size();
		m_isTransitioningSpeed = true;
	}

	s_CurrentTime += m_deltaTime;

	if (s_CurrentTime > s_SpeedTransitionTime)
		s_CurrentTime = s_SpeedTransitionTime;

	m_character->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(s_StartingMaxSpeed, m_maxSpeed, s_CurrentTime / s_SpeedTransitionTime);

	if (s_CurrentTime == s_SpeedTransitionTime)
		m_isTransitioningSpeed = false;
}

void UTerrainManager::RockTerrainFirstStep(void)
{
	m_terrainType = ETerrainType::Rock;
	m_character->mc_CameraManager->OnTerrainChange(m_terrainType);

	UpdateCharacterSettings		();
	CharacterMoveSpeedTransition(true);

	m_character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	m_character->GetCharacterMovement()->bOrientRotationToMovement     = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ROCK"));
	}	
}

void UTerrainManager::SandTerrainFirstStep(void)
{
	m_terrainType = ETerrainType::Sand;
	m_character->mc_CameraManager->OnTerrainChange(m_terrainType);

	UpdateCharacterSettings		();
	CharacterMoveSpeedTransition(true);

	m_character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	m_character->GetCharacterMovement()->bOrientRotationToMovement     = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("SAND"));
	}
}

void UTerrainManager::WaterTerrainFirstStep(void)
{
	m_lockSpeed      = true;
	m_bufferVelocity = m_character->GetCharacterMovement()->Velocity;

	m_terrainType = ETerrainType::Water;
	m_character->mc_CameraManager->OnTerrainChange(m_terrainType);

	UpdateCharacterSettings		();
	CharacterMoveSpeedTransition(true);

	m_character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	m_character->GetCharacterMovement()->bOrientRotationToMovement     = false;
	m_character->LerpForwardSpeed    (m_character->GetVelocity().Size() / m_waterSettings.Speed, 0.0f, false);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("WATER"));
	}	
}

void UTerrainManager::CorridorTerrainFirstStep(void)
{
	m_terrainType = ETerrainType::Corridor;
	m_character->mc_CameraManager->OnTerrainChange(m_terrainType);

	UpdateCharacterSettings();
	CharacterMoveSpeedTransition(true);

	m_character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	m_character->GetCharacterMovement()->bOrientRotationToMovement     = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("CORRIDOR"));
	}
}

void UTerrainManager::NeutralTerrainFirstStep(void)
{
	m_terrainType = ETerrainType::Neutral;
	m_character->mc_CameraManager->OnTerrainChange(m_terrainType);

	UpdateCharacterSettings		();
	CharacterMoveSpeedTransition(true);

	m_character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	m_character->GetCharacterMovement()->bOrientRotationToMovement     = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("NEUTRAL"));
	}
}

void UTerrainManager::SetInGameGUI(UW_InGameUI* inGameGUI) {
	m_inGameUIAttached = inGameGUI;
}

void UTerrainManager::SetRockTerrainSettings(const TerrainSettings& settings)     {
	UpdateTerrainSettings(settings, m_rockSettings);
}

void UTerrainManager::SetSandTerrainSettings(const TerrainSettings& settings)     {
	UpdateTerrainSettings(settings, m_sandSettings);
}

void UTerrainManager::SetWaterTerrainSettings(const TerrainSettings& settings)    {
	UpdateTerrainSettings(settings, m_waterSettings);
}

void UTerrainManager::SetCorridorTerrainSettings(const TerrainSettings& settings) {
	UpdateTerrainSettings(settings, m_corridorSettings);
}

void UTerrainManager::SetNeutralTerrainSettings(const TerrainSettings& settings)  {
	UpdateTerrainSettings(settings, m_neutralSettings);
}

void UTerrainManager::UpdateTerrainSettings(const TerrainSettings& in, TerrainSettings& out)
{
	out.Speed           = in.Speed;
	out.AirControl      = in.AirControl;
	out.Acceleration    = in.Acceleration;
	out.JumpZVelocity   = in.JumpZVelocity;
	out.FallingFriction = in.FallingFriction;
}

void UTerrainManager::UpdateCharacterSettings()
{
	float maxSpeed        = 0.0f;
	float airControl      = 0.0f;
	float acceleration    = 0.0f;
	float jumpZVelocity   = 0.0f;
	float fallingFriction = 0.0f;

	if (m_terrainType == ETerrainType::Rock)
	{
		maxSpeed        = m_rockSettings.Speed;
		airControl      = m_rockSettings.AirControl;
		acceleration    = m_rockSettings.Acceleration;
		jumpZVelocity   = m_rockSettings.JumpZVelocity;
		fallingFriction = m_rockSettings.FallingFriction;
	}
	else if (m_terrainType == ETerrainType::Sand)
	{
		maxSpeed        = m_sandSettings.Speed;
		airControl      = m_sandSettings.AirControl;
		acceleration    = m_sandSettings.Acceleration;
		jumpZVelocity   = m_sandSettings.JumpZVelocity;
		fallingFriction = m_sandSettings.FallingFriction;
	}
	else if (m_terrainType == ETerrainType::Water)
	{
		maxSpeed        = m_waterSettings.Speed;
		airControl      = m_waterSettings.AirControl;
		acceleration    = m_waterSettings.Acceleration;
		jumpZVelocity   = m_waterSettings.JumpZVelocity;
		fallingFriction = m_waterSettings.FallingFriction;
	}
	else if (m_terrainType == ETerrainType::Corridor)
	{
		maxSpeed        = m_corridorSettings.Speed;
		airControl      = m_corridorSettings.AirControl;
		acceleration    = m_corridorSettings.Acceleration;
		jumpZVelocity   = m_corridorSettings.JumpZVelocity;
		fallingFriction = m_corridorSettings.FallingFriction;
	}
	else if (m_terrainType == ETerrainType::Neutral)
	{
		maxSpeed        = m_neutralSettings.Speed;
		airControl      = m_neutralSettings.AirControl;
		acceleration    = m_neutralSettings.Acceleration;
		jumpZVelocity   = m_neutralSettings.JumpZVelocity;
		fallingFriction = m_neutralSettings.FallingFriction;
	}

	m_maxSpeed = maxSpeed;
	m_character->GetCharacterMovement()->MaxWalkSpeed           = maxSpeed;
	m_character->GetCharacterMovement()->AirControl             = airControl;
	m_character->GetCharacterMovement()->JumpZVelocity          = jumpZVelocity;
	m_character->GetCharacterMovement()->MaxAcceleration        = acceleration;
	m_character->GetCharacterMovement()->FallingLateralFriction = fallingFriction;
}

void UTerrainManager::SetRockMomemtumSettings(const RockMomemtumSettings& settings)
{
	m_momemtumSettings.IsActive = settings.IsActive;
	m_momemtumSettings.IsSquare = settings.IsSquare;										  
	m_momemtumSettings.MinValue = settings.MinValue;									      
}																						      
																						      
const TerrainSettings& UTerrainManager::GetRockTerrainSettings() const       {
	return m_rockSettings;																      
}																						      
																						      
const TerrainSettings& UTerrainManager::GetSandTerrainSettings() const       {
	return m_sandSettings;																      
}																						      
																						      
const TerrainSettings&	UTerrainManager::GetWaterTerrainSettings() const      {
	return m_waterSettings;																      
}																						      
																						      
const TerrainSettings&	UTerrainManager::GetNeutralTerrainSettings() const    {
	return m_neutralSettings;																  
}																							  
																							  
const TerrainSettings&	UTerrainManager::GetCorridorTerrainSettings() const   {
	return m_corridorSettings;
}

const RockMomemtumSettings& UTerrainManager::GetRockMomemtumSettings() const {
	return m_momemtumSettings;
}

UTerrainManager::ETerrainType UTerrainManager::GetTerrainType() const {
	return m_terrainType;
}

FString	UTerrainManager::GetTerrainTypeStringRepresentation() const 
{
	switch (m_terrainType)
	{
		case ETerrainType::Rock:     { return FString("ROCK");     }
		case ETerrainType::Sand:     { return FString("SAND");     }
		case ETerrainType::Water:    { return FString("WATER");    }
		case ETerrainType::Corridor: { return FString("CORRIDOR"); }
		case ETerrainType::Neutral:  { return FString("NEUTRAL");  }
	}

	return FString("NEUTRAL");
}

const FVector& UTerrainManager::GetTerrainNormal() const {
	return m_terrainNormal;
}