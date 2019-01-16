/// \file       GAttunedModel.cpp
/// \date       14/01/2019
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Model/AttunedModel.h"

#define LOCTEXT_NAMESPACE "AttunedModel"

#define COMMIT_IF_DIRTY(Cache, Result, Counter)		\
if(Cache.m_dirty)									\
{													\
	Result = Result && SerializeChanges(Cache);     \
	++Counter;										\
}													\

#define REVERT_IF_DIRTY(Cache, Result, Counter)		\
if(Cache.m_dirty)									\
{													\
	Result = Result && DeserializeChanges(Cache);	\
	++Counter;										\
}													\

void AttunedModel::Initialize()
{
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Initializing Tool model..."));

	bool  result  = true;
	int32 counter = 0;
	REVERT_IF_DIRTY(m_cameraDataCache,        result, counter)
	REVERT_IF_DIRTY(m_commonDataRockCache,    result, counter)
	REVERT_IF_DIRTY(m_commonDataSandCache,    result, counter)
	REVERT_IF_DIRTY(m_commonDataWaterCache,   result, counter)
	REVERT_IF_DIRTY(m_commonDataNeutralCache, result, counter)

	if (result)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] %d cache(s) was/were reverted."), counter);
		if (counter == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Attuned] Caches were already up-to-date."));
		}

		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Tool model initialized."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Tool model initialized with errors."));
	}
}

void AttunedModel::Destroy()
{
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Tool model destroyed."));
}

/// \brief	Updates the internal cache data from a structure
///			Marks the structure dirty
/// \param  The data structure to update
void AttunedModel::UpdateCache(const CameraData& data)
{
	m_cameraDataCache.m_dirty					  = true;
	m_cameraDataCache.m_maxArmLenghtValue		  = data.m_maxArmLenghtValue;
	m_cameraDataCache.m_maxTimeFromLastInputValue = data.m_maxTimeFromLastInputValue;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Camera data cache updated."));
}

void AttunedModel::UpdateCache(const CommonData& in, CommonData& out)
{
	out.m_dirty				   = true;
	out.m_fallingFrictionValue = in.m_fallingFrictionValue;
	out.m_airControlValue	   = in.m_airControlValue;
	out.m_jumpZVelocityValue   = in.m_jumpZVelocityValue;
	out.m_dashCooldownValue    = in.m_dashCooldownValue;
	out.m_accelerationValue    = in.m_accelerationValue;
	out.m_maxSpeedValue        = in.m_maxSpeedValue;
}

void AttunedModel::UpdateCache(const CommonDataRock& data)
{
	UpdateCache(data, m_commonDataRockCache);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Common data rock cache updated."));
}

void AttunedModel::UpdateCache(const CommonDataSand& data)
{
	UpdateCache(data, m_commonDataSandCache);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Common data sand cache updated."));
}

void AttunedModel::UpdateCache(const CommonDataWater& data)
{
	UpdateCache(data, m_commonDataWaterCache);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Common data water cache updated."));
}

void AttunedModel::UpdateCache(const CommonDataNeutral&	data)
{
	UpdateCache(data, m_commonDataNeutralCache);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Common data neutral cache updated."));
}

/// \brief	Saves persistently all data marked as dirty on the disk
/// \return True if no error occurs, else false
bool AttunedModel::CommitChanges()
{
	bool  result  = true;
	int32 counter = 0;
	COMMIT_IF_DIRTY(m_cameraDataCache,        result, counter)
	COMMIT_IF_DIRTY(m_commonDataRockCache,    result, counter)
	COMMIT_IF_DIRTY(m_commonDataSandCache,    result, counter)
	COMMIT_IF_DIRTY(m_commonDataWaterCache,   result, counter)
	COMMIT_IF_DIRTY(m_commonDataNeutralCache, result, counter)

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] %d cache(s) was/were committed."), counter);
	if (counter == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Caches were already up-to-date."));
	}

	return result;
}

/// \brief	Reverts all changes stored in the cache to the values on the disk
/// \return True if no error occurs, else false
bool AttunedModel::RevertChanges()
{
	bool  result = true;
	int32 counter = 0;
	REVERT_IF_DIRTY(m_cameraDataCache,        result, counter)
	REVERT_IF_DIRTY(m_commonDataRockCache,    result, counter)
	REVERT_IF_DIRTY(m_commonDataSandCache,    result, counter)
	REVERT_IF_DIRTY(m_commonDataWaterCache,   result, counter)
	REVERT_IF_DIRTY(m_commonDataNeutralCache, result, counter)

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] %d cache(s) was/were reverted."), counter);
	if (counter == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Caches were already up-to-date."));
	}

	return result;
}

/// \brief	Serializes changes from the camera data
/// \param  data The camera data to save on disk
/// \return True if no error occurs, else false
bool AttunedModel::SerializeChanges(const CameraData& data)
{
	FBufferArchive Ar;
	Ar.SetIsSaving	  (true);
	Ar.SetIsPersistent(true);

	Ar << (float)data.m_maxArmLenghtValue;
	Ar << (float)data.m_maxTimeFromLastInputValue;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_maxArmLenghtValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_maxTimeFromLastInputValue);

	return WriteArchive(Ar, data.m_archiveName);
}

bool AttunedModel::SerializeChanges(const CommonData& data)
{
	FBufferArchive Ar;
	Ar.SetIsSaving	  (true);
	Ar.SetIsPersistent(true);

	Ar << (float)data.m_fallingFrictionValue;
	Ar << (float)data.m_airControlValue;
	Ar << (float)data.m_jumpZVelocityValue;
	Ar << (float)data.m_dashCooldownValue;
	Ar << (float)data.m_accelerationValue;
	Ar << (float)data.m_maxSpeedValue;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_fallingFrictionValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_airControlValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_jumpZVelocityValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_dashCooldownValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_accelerationValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_maxSpeedValue);

	return WriteArchive(Ar, data.m_archiveName);
}

bool AttunedModel::DeserializeChanges(CameraData& data)
{
	TArray<uint8> Bytes;

	// Reads the corresponding archive on the disk
	// Stores the read bytes in the bytes array
	if (!ReadArchive(Bytes, data.m_archiveName))
	{
		return false;
	}

	FMemoryReader Binary = FMemoryReader(Bytes, true);
	Binary.Seek(0);

	data.m_dirty = false;
	Binary << data.m_maxArmLenghtValue;
	Binary << data.m_maxTimeFromLastInputValue;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_maxArmLenghtValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_maxTimeFromLastInputValue);

	return true;
}

bool AttunedModel::DeserializeChanges(CommonData& data)
{
	TArray<uint8> Bytes;

	if (!ReadArchive(Bytes, data.m_archiveName))
	{
		return false;
	}

	FMemoryReader Binary = FMemoryReader(Bytes, true);
	Binary.Seek(0);

	data.m_dirty = false;
	Binary << data.m_fallingFrictionValue;
	Binary << data.m_airControlValue;
	Binary << data.m_jumpZVelocityValue;
	Binary << data.m_dashCooldownValue;
	Binary << data.m_accelerationValue;
	Binary << data.m_maxSpeedValue;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_fallingFrictionValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_airControlValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_jumpZVelocityValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_dashCooldownValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_accelerationValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_maxSpeedValue);

	return true;
}

/// \brief  Saves the archive on disk
/// \param  Ar   The archive to save
/// \param  Name The name of the archive
/// \return True if no error occurs, else false
bool AttunedModel::WriteArchive(FBufferArchive& Ar, const TCHAR* Name)
{
	// Checked if there are data in the archive
	if (Ar.Num() <= 0) 
	{
		return false;
	}

	// Creating the path (Located in the user plugin folder)
	FString displayablePath = FString(TEXT("AttunedTool/Resources/Archives/")) + Name;
	FString fullPath        = FPaths::ProjectPluginsDir() + displayablePath;

	// Save binaries to disk
	bool result = FFileHelper::SaveArrayToFile(Ar, *fullPath, &IFileManager::Get(), true);

	if (result) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] %li bytes were saved in %s."), Ar.TotalSize(), *displayablePath);
	}

	// Empty the buffer's contents
	Ar.FlushCache();
	Ar.Empty();

	return result;
}

/// \brief	Reads an archive from the disk
/// \param  Bytes The bytes array to store the read data
/// \param  Name  The name of the archive to read
/// \return True if no error occurs, else false
bool AttunedModel::ReadArchive(TArray<uint8>& Bytes, const TCHAR* Name)
{
	// Creating the path (Located in the user plugin folder)
	FString displayablePath = FString(TEXT("AttunedTool/Resources/Archives/")) + Name;
	FString fullPath		= FPaths::ProjectPluginsDir() + displayablePath;

	// Load disk data into the binary array
	if (!FFileHelper::LoadFileToArray(Bytes, *fullPath, EFileRead::FILEREAD_None))
	{
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] %li bytes were saved in %s."), Bytes.Num(), *displayablePath);
	
	// The file is empty ...
	if (Bytes.Num() <= 0)
	{
		return false;
	}

	return true;
}

#undef LOCTEXT_NAMESPACE