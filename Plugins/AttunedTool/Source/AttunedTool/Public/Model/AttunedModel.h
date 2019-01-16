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

#define OFFSET(T, X) ((size_t)&((T*)0)->X)

struct Serializable
{
	Serializable(int32 offset, int32 componentCount, const TCHAR* archiveName)
	: m_offset			(offset)
	, m_componentCount	(componentCount)
	, m_archiveName		(archiveName)
	{
		// None
	}

	const int32  m_offset;
	const int32  m_componentCount;
	const TCHAR* m_archiveName;
};

struct CameraData : public Serializable
{
	CameraData() 
	: Serializable(OFFSET(CameraData, m_maxArmLenghtValue), 2, TEXT("CameraDataArchive"))
	, m_maxArmLenghtValue(0.0f)
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

	bool CommitChanges(const CameraData& data);

	template<class T> bool SerializeData   (const T& data) const;
	template<class T> bool DeserializeData (T& data)       const;

private:

	// Cached model
	CameraData m_cameraDataCache;

private:

	friend class GAttunedTool;

	/// \brief Initialize the singleton
	void Initialize	();

	/// \brief Destroy the singleton
	void Destroy	();

	/// \brief Tests the result of a serialization operation
	void CheckSerialization(bool ret, const TCHAR* message) const;
};

#include "Model/AttunedModel.inl"

#endif // ATTUNED_TOOL_ATTUNED_MODEL_H_