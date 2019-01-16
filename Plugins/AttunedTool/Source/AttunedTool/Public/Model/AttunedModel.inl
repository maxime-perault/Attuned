/// \file       AttunedModel.inl
/// \date       14/01/2019
/// \project    Attuned
/// \package    AttunedTool
/// \author     Vincent STEHLY--CALISTO

template<class T>
bool AttunedModel::SerializeData(const T& data) const
{
	static_assert(std::is_base_of<Serializable, T>::value, 
		"The type T must inherit from Serializable");

	FBufferArchive Ar;
	Ar.SetIsSaving(true);
	Ar.SetIsPersistent(true);
	
	// Getting the pointer on the data section
	const float* p_data = reinterpret_cast<const float*>(
						  reinterpret_cast<const char* >(&data) + data.m_offset);

	// Serializing data into the archive
	for (int32 i = 0; i < data.m_componentCount; ++i) {
		Ar << (float)p_data[i];
		UE_LOG(LogTemp, Warning, TEXT("[Attuned] Serialized [%d] : %lf"), i, (float)p_data[i]);
	}

	// No elements were serialized
	if (Ar.Num() <= 0) {
		return false;
	}

	// Creating the path
	FString displayablePath = FString(TEXT("AttunedTool/Resources/Archives/")) + data.m_archiveName;
	FString fullPath        = FPaths::GamePluginsDir() + displayablePath;
	
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

template<class T>
bool AttunedModel::DeserializeData(T& data) const
{
	static_assert(std::is_base_of<Serializable, T>::value, 
		"The type T must inherit from Serializable");

	TArray<uint8> byteArray;

	// Load disk data into the binary array
	if (!FFileHelper::LoadFileToArray(byteArray, data.m_archiveName, EFileRead::FILEREAD_None)) {
		return false;
	}

	// The file is empty ...
	if (byteArray.Num() <= 0) {
		return false;
	}

	// Memory reader is the archive that we're going to use in order to read the loaded data
	FMemoryReader binaryData = FMemoryReader(byteArray, true);
	binaryData.Seek(0);

	// Getting the pointer on the data section
	float* p_data = reinterpret_cast<float*>(
					reinterpret_cast<char* >(&data) + data.m_offset);

	// Deserializing all data into the host structure
	for (int32 i = 0; i < data.m_componentCount; ++i) {
		binaryData << p_data[i];
	}

	UE_LOG(LogTemp, Warning, TEXT("[Attuned] Archive %s deserialized."), data.m_archiveName);

	binaryData.FlushCache();
	binaryData.Close();

	byteArray.Empty();

	return true;
}