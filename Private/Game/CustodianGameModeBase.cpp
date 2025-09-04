// Copyright Sertim all rights reserved


#include "Game/CustodianGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CustodianSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Game/CustodianGameInstance.h"
#include "EngineUtils.h"
#include "Interaction/SaveInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "GameFramework/Character.h"

UCustodianSaveGame* ACustodianGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(CustodianSaveGameClass);
	}
	UCustodianSaveGame* CustodianSaveGame = Cast<UCustodianSaveGame>(SaveGameObject);
	return CustodianSaveGame;
}

void ACustodianGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

UCustodianSaveGame* ACustodianGameModeBase::RetrieveInGameSaveData()
{
	UCustodianGameInstance* CustodianGameInstance = Cast<UCustodianGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = CustodianGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = CustodianGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void ACustodianGameModeBase::SaveInGameProgressData(UCustodianSaveGame* SaveObject)
{
	UCustodianGameInstance* CustodianGameInstance = Cast<UCustodianGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = CustodianGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = CustodianGameInstance->LoadSlotIndex;
	CustodianGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void ACustodianGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UCustodianGameInstance* CustodianGI = Cast<UCustodianGameInstance>(GetGameInstance());
	check(CustodianGI);

	if (UCustodianSaveGame* SaveGame = GetSaveSlotData(CustodianGI->LoadSlotName, CustodianGI->LoadSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}

		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty(); // clear it out. we'll fill it in with "actors"

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(SavedActor.Bytes);

			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;

			Actor->Serialize(Archive);

			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, CustodianGI->LoadSlotName, CustodianGI->LoadSlotIndex);
	}
}

void ACustodianGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UCustodianGameInstance* CustodianGI = Cast<UCustodianGameInstance>(GetGameInstance());
	check(CustodianGI);

	if (UGameplayStatics::DoesSaveGameExist(CustodianGI->LoadSlotName, CustodianGI->LoadSlotIndex))
	{
		UCustodianSaveGame* SaveGame = Cast<UCustodianSaveGame>(UGameplayStatics::LoadGameFromSlot(CustodianGI->LoadSlotName, CustodianGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load Slot"));
			return;
		}

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive); // converts binary bytes back into variables

					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

FString ACustodianGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

AActor* ACustodianGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UCustodianGameInstance* CustodianGameInstance = Cast<UCustodianGameInstance>(GetGameInstance());

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == CustodianGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void ACustodianGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	UCustodianSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

void ACustodianGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);
}
