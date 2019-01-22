/// \file       GAttunedModel.cpp
/// \date       14/01/2019
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Model/AttunedModel.h"

#define LOCTEXT_NAMESPACE "AttunedModel"

#define COMMIT_IF_DIRTY(Cache, Result, Counter, USER)   \
if(Cache.m_dirty)										\
{														\
	Result = Result && SerializeChanges(Cache, USER);	\
	++Counter;											\
}														\

#define REVERT_IF_DIRTY(Cache, Result, Counter, USER)	\
if(Cache.m_dirty)										\
{														\
	Result = Result && DeserializeChanges(Cache, USER);	\
	++Counter;											\
}														\

void AttunedModel::Initialize()
{
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Initializing Tool model..."));

	bool  result  = true;
	int32 counter = 0;

	// Reverting preference first
	REVERT_IF_DIRTY(m_profilePreferenceDataCache, result, counter, true)

	REVERT_IF_DIRTY(m_cameraDataCache,            result, counter, false)
	REVERT_IF_DIRTY(m_commonDataRockCache,        result, counter, false)
	REVERT_IF_DIRTY(m_commonDataSandCache,        result, counter, false)
	REVERT_IF_DIRTY(m_commonDataWaterCache,       result, counter, false)
	REVERT_IF_DIRTY(m_commonDataNeutralCache,     result, counter, false)
	REVERT_IF_DIRTY(m_rockMomemtumDataCache,      result, counter, false)
	
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

void AttunedModel::UpdateCache(const ProfilePreferenceData& data)
{
	UpdateCache(data, m_profilePreferenceDataCache);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Profile preference data cache updated."));
}

void AttunedModel::UpdateCache(const RockMomemtumData& data)
{
	UpdateCache(data, m_rockMomemtumDataCache);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Rock momemtum data cache updated."));
}

void AttunedModel::UpdateCache(const CommonData& in, CommonData& out)
{
	out.m_dirty                = true;
	out.m_fallingFrictionValue = in.m_fallingFrictionValue;
	out.m_airControlValue      = in.m_airControlValue;
	out.m_jumpZVelocityValue   = in.m_jumpZVelocityValue;
	out.m_dashCooldownValue    = in.m_dashCooldownValue;
	out.m_accelerationValue    = in.m_accelerationValue;
	out.m_maxSpeedValue        = in.m_maxSpeedValue;
}

void AttunedModel::UpdateCache(const ProfilePreferenceData& in, ProfilePreferenceData& out)
{
	out.m_dirty        = true;
	out.m_profileIndex = in.m_profileIndex;
	out.m_profileName  = in.m_profileName;
}

void AttunedModel::UpdateCache(const RockMomemtumData& in, RockMomemtumData& out)
{
	out.m_dirty           = true;
	out.m_bActiveMomemtum = in.m_bActiveMomemtum;
	out.m_bSquareMomemtum = in.m_bSquareMomemtum;
	out.m_minMomemtum     = in.m_minMomemtum;
}

/// \brief	Saves persistently all data marked as dirty on the disk
/// \return True if no error occurs, else false
bool AttunedModel::CommitChanges()
{
	bool  result  = true;
	int32 counter = 0;
	COMMIT_IF_DIRTY(m_cameraDataCache,        result, counter, false)
	COMMIT_IF_DIRTY(m_commonDataRockCache,    result, counter, false)
	COMMIT_IF_DIRTY(m_commonDataSandCache,    result, counter, false)
	COMMIT_IF_DIRTY(m_commonDataWaterCache,   result, counter, false)
	COMMIT_IF_DIRTY(m_commonDataNeutralCache, result, counter, false)
	COMMIT_IF_DIRTY(m_rockMomemtumDataCache,  result, counter, false)

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] %d cache(s) was/were committed."), counter);
	if (counter == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Caches were already up-to-date."));
	}

	return result;
}

bool AttunedModel::CommitProfilePreference()
{
	bool  result  = true;
	int32 counter = 0;
	COMMIT_IF_DIRTY(m_profilePreferenceDataCache, result, counter, true)

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] %d cache(s) was/were committed."), counter);
	if (counter == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Caches were already up-to-date."));
	}

	return result;
}

void AttunedModel::InvalidateAllCaches()
{
	m_cameraDataCache.m_dirty        = true;
	m_commonDataRockCache.m_dirty    = true;
	m_commonDataSandCache.m_dirty    = true;
	m_commonDataWaterCache.m_dirty   = true;
	m_commonDataNeutralCache.m_dirty = true;
	m_rockMomemtumDataCache.m_dirty  = true;
}

/// \brief	Reverts all changes stored in the cache to the values on the disk
/// \return True if no error occurs, else false
bool AttunedModel::RevertChanges()
{
	bool  result = true;
	int32 counter = 0;
	REVERT_IF_DIRTY(m_cameraDataCache,        result, counter, false)
	REVERT_IF_DIRTY(m_commonDataRockCache,    result, counter, false)
	REVERT_IF_DIRTY(m_commonDataSandCache,    result, counter, false)
	REVERT_IF_DIRTY(m_commonDataWaterCache,   result, counter, false)
	REVERT_IF_DIRTY(m_commonDataNeutralCache, result, counter, false)
	REVERT_IF_DIRTY(m_rockMomemtumDataCache,  result, counter, false)

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] %d cache(s) was/were reverted."), counter);
	if (counter == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Caches were already up-to-date."));
	}

	return result;
}

/// \brief	Serializes changes from the camera data
/// \param  data The camera data to save on disk
/// \param  bUser Tells if the file is user specific (false by default)
/// \return True if no error occurs, else false
bool AttunedModel::SerializeChanges(const CameraData& data, bool bUser)
{
	FBufferArchive Ar;
	Ar.SetIsSaving	  (true);
	Ar.SetIsPersistent(true);

	Ar << (float)data.m_maxArmLenghtValue;
	Ar << (float)data.m_maxTimeFromLastInputValue;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_maxArmLenghtValue);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_maxTimeFromLastInputValue);

	return WriteArchive(Ar, data.m_archiveName, bUser);
}

bool AttunedModel::SerializeChanges(const CommonData& data, bool bUser)
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

	return WriteArchive(Ar, data.m_archiveName, bUser);
}

bool AttunedModel::SerializeChanges(const ProfilePreferenceData& data, bool bUser)
{
	FBufferArchive Ar;
	Ar.SetIsSaving    (true);
	Ar.SetIsPersistent(true);
	
	Ar << (int32)  data.m_profileIndex;
	Ar << (FString&)data.m_profileName;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %d"),  data.m_profileIndex);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %s"), *data.m_profileName);

	return WriteArchive(Ar, data.m_archiveName, bUser);
}

bool AttunedModel::SerializeChanges(const RockMomemtumData& data, bool bUser)
{
	FBufferArchive Ar;
	Ar.SetIsSaving    (true);
	Ar.SetIsPersistent(true);

	Ar << (bool) data.m_bActiveMomemtum;
	Ar << (bool) data.m_bSquareMomemtum;
	Ar << (float)data.m_minMomemtum;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %d"),  data.m_bActiveMomemtum);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %d"),  data.m_bSquareMomemtum);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized value : %lf"), data.m_minMomemtum);

	return WriteArchive(Ar, data.m_archiveName, bUser);
}

bool AttunedModel::DeserializeChanges(CameraData& data, bool bUser)
{
	TArray<uint8> Bytes;

	// Reads the corresponding archive on the disk
	// Stores the read bytes in the bytes array
	if (!ReadArchive(Bytes, data.m_archiveName, bUser))
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

bool AttunedModel::DeserializeChanges(CommonData& data, bool bUser)
{
	TArray<uint8> Bytes;

	if (!ReadArchive(Bytes, data.m_archiveName, bUser))
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

bool AttunedModel::DeserializeChanges(ProfilePreferenceData& data, bool bUser)
{
	TArray<uint8> Bytes;

	if (!ReadArchive(Bytes, data.m_archiveName, bUser))
	{
		return false;
	}

	FMemoryReader Binary = FMemoryReader(Bytes, true);
	Binary.Seek(0);

	data.m_dirty = false;
	data.m_profileName.Reset();

	Binary << data.m_profileIndex;
	Binary << data.m_profileName;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %d"),  data.m_profileIndex);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %s"), *data.m_profileName);

	return true;
}

bool AttunedModel::DeserializeChanges(RockMomemtumData& data, bool bUser)
{
	TArray<uint8> Bytes;

	if (!ReadArchive(Bytes, data.m_archiveName, bUser))
	{
		return false;
	}

	FMemoryReader Binary = FMemoryReader(Bytes, true);
	Binary.Seek(0);

	data.m_dirty = false;
	Binary << data.m_bActiveMomemtum;
	Binary << data.m_bSquareMomemtum;
	Binary << data.m_minMomemtum;

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %d"),  data.m_bActiveMomemtum);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %d"),  data.m_bSquareMomemtum);
	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Deserialized value : %lf"), data.m_minMomemtum);

	return true;
}

/// \brief  Saves the archive on disk
/// \param  Ar   The archive to save
/// \param  Name The name of the archive
/// \param  bUser Tells if the file is user specific (false by default)
/// \return True if no error occurs, else false
bool AttunedModel::WriteArchive(FBufferArchive& Ar, const TCHAR* Name, bool bUser)
{
	// Checked if there are data in the archive
	if (Ar.Num() <= 0) 
	{
		return false;
	}

	FString basePath;
	FString userPath;
	FString fullPath;

	if (bUser)
	{
		basePath = FPaths::ProjectIntermediateDir();
		userPath = FString(TEXT("Profile/")) + Name;
	}
	else
	{
		// Creating the path (Located in the user plugin folder)
		basePath = FPaths::ProjectPluginsDir() + FString(TEXT("/AttunedTool/Resources/Archives/"));
		userPath = m_profilePreferenceDataCache.m_profileName + FString(TEXT("/")) + Name;
	}

	// Located in the intermediate folder or in user plugin directory
	fullPath = basePath + userPath;

	// Save binaries to disk
	bool result = FFileHelper::SaveArrayToFile(Ar, *fullPath, &IFileManager::Get(), true);

	if (result) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] %li bytes were saved in %s."), Ar.TotalSize(), *(basePath + userPath));
	}

	// Empty the buffer's contents
	Ar.FlushCache();
	Ar.Empty();

	return result;
}

/// \brief	Reads an archive from the disk
/// \param  Bytes The bytes array to store the read data
/// \param  Name  The name of the archive to read
/// \param  bUser Tells if the file is user specific (false by default)
/// \return True if no error occurs, else false
bool AttunedModel::ReadArchive(TArray<uint8>& Bytes, const TCHAR* Name, bool bUser)
{
	FString basePath;
	FString userPath;
	FString fullPath;

	if (bUser)
	{
		basePath = FPaths::ProjectIntermediateDir();
		userPath = FString(TEXT("Profile/")) + Name;

		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Base : %s"), *(basePath));
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] User : %s"), *(userPath));
	}
	else
	{
		// Creating the path (Located in the user plugin folder)
		basePath = FPaths::ProjectPluginsDir() + FString(TEXT("/AttunedTool/Resources/Archives/"));
		userPath = m_profilePreferenceDataCache.m_profileName + FString(TEXT("/")) + Name;
	}

	// Located in the intermediate folder or in user plugin directory
	fullPath = basePath + userPath;

	// Load disk data into the binary array
	if (!FFileHelper::LoadFileToArray(Bytes, *fullPath, EFileRead::FILEREAD_None))
	{
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] %li bytes were read in %s."), Bytes.Num(), *(basePath + userPath));
	
	// The file is empty ...
	if (Bytes.Num() <= 0)
	{
		return false;
	}

	return true;
}

#undef LOCTEXT_NAMESPACE