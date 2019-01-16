/// \file       GAttunedModel.cpp
/// \date       14/01/2019
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

#include "Model/AttunedModel.h"

#include "FileHelper.h"
#include "MemoryReader.h"
#include "BufferArchive.h"

#define LOCTEXT_NAMESPACE "AttunedModel"

void AttunedModel::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("[Attuned] Tool model initialized."));

	// Updating the cached model from disk
	CheckSerialization(DeserializeData<CameraData>(m_cameraDataCache), TEXT("CameraDataCache"));
}

void AttunedModel::Destroy()
{
	UE_LOG(LogTemp, Log, TEXT("[Attuned] Tool model destroyed."));

	// Saving all cached data on the disk
	CheckSerialization(SerializeData<CameraData>(m_cameraDataCache), TEXT("CameraDataCache"));
}

void AttunedModel::CheckSerialization(bool ret, const TCHAR* message) const
{
	if (!ret)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialization failure in archive : %s"), message);
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Data may be corrupted or incorrect."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[Attuned] Archive %s successfully serialized."), message);
	}
}

bool AttunedModel::CommitChanges(const CameraData& data)
{
	m_cameraDataCache.m_maxArmLenghtValue         = data.m_maxArmLenghtValue;
	m_cameraDataCache.m_maxTimeFromLastInputValue = data.m_maxTimeFromLastInputValue;

	return SerializeData(m_cameraDataCache);
}

#undef LOCTEXT_NAMESPACE