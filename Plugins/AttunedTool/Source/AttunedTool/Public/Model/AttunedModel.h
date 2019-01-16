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

/// \brief This is the model of the MVC pattern 
/// \class AttunedModel
class AttunedModel
{
public:

	template<class T>	const T*			GetCache() const { return nullptr;			  }
	template<>			const CameraData*	GetCache() const { return &m_cameraDataCache; }

	/// \brief	Updates the internal cache data from a structure
	///			Marks the structure dirty
	/// \param  The data structure to update
	void UpdateCache(const CameraData& data);

	/// \brief	Saves persistently all data marked as dirty on the disk
	/// \return True if no error occurs, else false
	bool CommitChanges();

	/// \brief	Reverts all changes stored in the cache to the values on the disk
	/// \return True if no error occurs, else false
	bool RevertChanges();

private:

	/// \brief	Serializes changes from the camera data
	/// \param  data The camera data to save on disk
	/// \return True if no error occurs, else false
	bool SerializeChanges(const CameraData& data);

	/// \brief	Deserializes changes from the camera archive
	/// \param  data The camera data to save on disk
	/// \return True if no error occurs, else false
	bool DeserializeChanges(CameraData& data);

	/// \brief  Saves the archive on disk
	/// \param  Ar   The archive to save
	/// \param  Name The name of the archive
	/// \return True if no error occurs, else false
	bool WriteArchive(FBufferArchive& Ar, const TCHAR* Name);

	/// \brief	Reads an archive from the disk
	/// \param  Bytes The bytes array to store the read data
	/// \param  Name  The name of the archive to read
	/// \return True if no error occurs, else false
	bool ReadArchive(TArray<uint8>& Bytes, const TCHAR* Name);

private:

	CameraData m_cameraDataCache; ///< Cached data model

private:

	friend class GAttunedTool;

	void Initialize	();
	void Destroy	();
};

#endif // ATTUNED_TOOL_ATTUNED_MODEL_H_