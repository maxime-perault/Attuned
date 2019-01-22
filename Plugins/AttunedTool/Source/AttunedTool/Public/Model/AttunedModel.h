/// \file       AttunedModel.h
/// \date       14/01/2019
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#ifndef ATTUNED_TOOL_ATTUNED_MODEL_H_
#define ATTUNED_TOOL_ATTUNED_MODEL_H_

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

#include "Archive.h"
#include "FileHelper.h"
#include "MemoryReader.h"
#include "BufferArchive.h"

struct Serializable
{
	Serializable(const TCHAR* archiveName)
	: m_archiveName	(archiveName)
	, m_dirty		(true)
	{
		// None
	}

	const TCHAR* m_archiveName;
	bool		 m_dirty;
};

struct CameraData : public Serializable
{
	CameraData() 
	: Serializable				 (TEXT("CameraDataArchive"))
	, m_maxArmLenghtValue		 (0.0f)
	, m_maxTimeFromLastInputValue(0.0f)
	{
		// None
	}

	float m_maxArmLenghtValue;
	float m_maxTimeFromLastInputValue;

	// Limits
	static const constexpr float MaxArmLenghtMinValue             = 1.0f;
	static const constexpr float MaxArmLenghtMaxValue             = 1000.0f;
	static const constexpr float MaxArmLenghtDefaultValue         = 1.0f;
	static const constexpr float MaxTimeFromLastInputMinValue     = 0.00f;
	static const constexpr float MaxTimeFromLastInputMaxValue     = 20.0f;
	static const constexpr float MaxTimeFromLastInputDefaultValue = 1.0f;
};

struct CommonData : public Serializable
{
	CommonData(const TCHAR* archiveName)
	: Serializable			(archiveName)
	, m_fallingFrictionValue(0.0f)
	, m_airControlValue		(0.0f)
	, m_jumpZVelocityValue	(0.0f)
	, m_dashCooldownValue	(0.0f)
	, m_accelerationValue	(0.0f)
	, m_maxSpeedValue		(0.0f)
	{
		// None
	}

	float m_fallingFrictionValue;
	float m_airControlValue;
	float m_jumpZVelocityValue;
	float m_dashCooldownValue;
	float m_accelerationValue;
	float m_maxSpeedValue;

	// Common limits
	static const constexpr float FallingFrictionMinValue	= 0.0f;
	static const constexpr float FallingFrictionMaxValue	= 500.0f;
	static const constexpr float AirControlMinValue			= 0.0f;
	static const constexpr float AirControlMaxValue			= 500.0f;
	static const constexpr float JumpZVelocityMinValue		= 0.0f;
	static const constexpr float JumpZVelocityMaxValue		= 5000.0f;
	static const constexpr float DashCooldownMinValue		= 0.0f;
	static const constexpr float DashCooldownMaxValue		= 20.0f;
	static const constexpr float AccelerationMinValue		= 0.0f;
	static const constexpr float AccelerationMaxValue		= 5000.0f;
	static const constexpr float MaxSpeedMinValue			= 0.0f;
	static const constexpr float MaxSpeedMaxValue			= 10000.0f;

	static void Copy(const CommonData& in, CommonData& out)
	{
		out.m_fallingFrictionValue = in.m_fallingFrictionValue;
		out.m_airControlValue      = in.m_airControlValue;
		out.m_jumpZVelocityValue   = in.m_jumpZVelocityValue;
		out.m_dashCooldownValue    = in.m_dashCooldownValue;
		out.m_accelerationValue    = in.m_accelerationValue;
		out.m_maxSpeedValue        = in.m_maxSpeedValue;
	}
};

struct CommonDataGeneric : public CommonData
{
	CommonDataGeneric()
	: CommonData(TEXT(""))
	{
		// None
	}
};

struct CommonDataNeutral : public CommonData
{
	CommonDataNeutral()
	: CommonData	(TEXT("CommonDataNeutralArchive"))
	{
		// None
	}
};

struct CommonDataRock : public CommonData
{
	CommonDataRock()
	: CommonData  (TEXT("CommonDataRockArchive"))
	{
		// None
	}
};

struct CommonDataSand : public CommonData
{
	CommonDataSand()
	: CommonData  (TEXT("CommonDataSandArchive"))
	{
		// None
	}
};

struct CommonDataWater : public CommonData
{
	CommonDataWater()
	: CommonData   (TEXT("CommonDataWaterArchive"))
	{
		// None
	}
};

struct ProfilePreferenceData : public Serializable
{
	ProfilePreferenceData()
	: Serializable  (TEXT("ProfilePreferenceData"))
	, m_profileIndex(0)
	, m_profileName ("Antoine")
	{
		// None
	}

	int32   m_profileIndex;
	FString m_profileName;
};

struct RockMomemtumData : public Serializable
{
	RockMomemtumData()
	: Serializable(TEXT("RockMomemtumData"))
	{
		// None
	}

	bool  m_bActiveMomemtum;
	bool  m_bSquareMomemtum;
	float m_minMomemtum;
};

/// \brief This is the model of the MVC pattern 
/// \class AttunedModel
class AttunedModel
{
public:

	template<class T>	const T*						GetCache() const { return nullptr;							}
	template<>			const CameraData*				GetCache() const { return &m_cameraDataCache;				}
	template<>			const CommonDataRock*			GetCache() const { return &m_commonDataRockCache;			}
	template<>			const CommonDataSand*			GetCache() const { return &m_commonDataSandCache;			}
	template<>			const CommonDataWater*			GetCache() const { return &m_commonDataWaterCache;			}
	template<>			const CommonDataNeutral*		GetCache() const { return &m_commonDataNeutralCache;		}
	template<>			const ProfilePreferenceData*	GetCache() const { return &m_profilePreferenceDataCache;	}
	template<>			const RockMomemtumData*			GetCache() const { return &m_rockMomemtumDataCache;			}

	/// \brief	Updates the internal cache data from a structure
	///			Marks the structure dirty
	/// \param  The data structure to update
	void UpdateCache(const CameraData&				data);
	void UpdateCache(const CommonDataRock&			data);
	void UpdateCache(const CommonDataSand&			data);
	void UpdateCache(const CommonDataWater&			data);
	void UpdateCache(const CommonDataNeutral&		data);
	void UpdateCache(const ProfilePreferenceData&	data);
	void UpdateCache(const RockMomemtumData&		data);

	/// \brief	Saves persistently all data marked as dirty on the disk
	/// \return True if no error occurs, else false
	bool CommitChanges();
	bool CommitProfilePreference();

	/// \brief	Reverts all changes stored in the cache to the values on the disk
	/// \return True if no error occurs, else false
	bool RevertChanges();

	/// \brief  Marks all caches as dirty
	void InvalidateAllCaches();

private:

	/// \brief Helper function to avoid duplicating code for 
	///		   all common terrain data structures
	void UpdateCache(const CommonData&            in, CommonData&            out);
	void UpdateCache(const ProfilePreferenceData& in, ProfilePreferenceData& out);
	void UpdateCache(const RockMomemtumData&      in, RockMomemtumData&      out);


	/// \brief	Serializes changes from the camera data
	/// \param  data The camera data to save on disk
	/// \param  bUser Tells if the file is user specific (false by default)
	/// \return True if no error occurs, else false
	bool SerializeChanges(const CameraData& data, bool bUser = false);
	bool SerializeChanges(const CommonData& data, bool bUser = false);
	bool SerializeChanges(const ProfilePreferenceData& data, bool bUser = false);
	bool SerializeChanges(const RockMomemtumData&      data, bool bUser = false);

	/// \brief	Deserializes changes from the camera archive
	/// \param  data The camera data to save on disk
	/// \param  bUser Tells if the file is user specific (false by default)
	/// \return True if no error occurs, else false
	bool DeserializeChanges(CameraData& data, bool bUser = false);
	bool DeserializeChanges(CommonData& data, bool bUser = false);
	bool DeserializeChanges(ProfilePreferenceData& data, bool bUser = false);
	bool DeserializeChanges(RockMomemtumData&      data, bool bUser = false);

	/// \brief  Saves the archive on disk
	/// \param  Ar   The archive to save
	/// \param  Name The name of the archive
	/// \param  bUser Tells if the file is user specific (false by default)
	/// \return True if no error occurs, else false
	bool WriteArchive(FBufferArchive& Ar, const TCHAR* Name, bool bUser = false);

	/// \brief	Reads an archive from the disk
	/// \param  Bytes The bytes array to store the read data
	/// \param  Name  The name of the archive to read
	/// \param  bUser Tells if the file is user specific (false by default)
	/// \return True if no error occurs, else false
	bool ReadArchive(TArray<uint8>& Bytes, const TCHAR* Name, bool bUser = false);

private:

	CameraData				m_cameraDataCache;
	CommonDataRock			m_commonDataRockCache;
	CommonDataSand			m_commonDataSandCache;
	CommonDataWater			m_commonDataWaterCache;
	CommonDataNeutral		m_commonDataNeutralCache;
	ProfilePreferenceData	m_profilePreferenceDataCache;
	RockMomemtumData		m_rockMomemtumDataCache;

private:

	friend class GAttunedTool;

	void Initialize	();
	void Destroy	();
};

#endif // ATTUNED_TOOL_ATTUNED_MODEL_H_