// Copyright Sertim all rights reserved


#include "Game/CustodianSaveGame.h"

FSavedMap UCustodianSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

bool UCustodianSaveGame::HasMap(const FString& InMapName)
{
	for (FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}
