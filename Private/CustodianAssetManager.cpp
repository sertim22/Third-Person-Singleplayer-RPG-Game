// Copyright Sertim all rights reserved


#include "CustodianAssetManager.h"
#include "CustodianGameplayTags.h"
#include "AbilitySystemGlobals.h"

UCustodianAssetManager& UCustodianAssetManager::Get()
{
	check(GEngine);

	UCustodianAssetManager* CustodianAssetManager = Cast<UCustodianAssetManager>(GEngine->AssetManager);
	return *CustodianAssetManager;
}

void UCustodianAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCustodianGameplayTags::InitializeNativeGameplayTags();

	//Required to use Target Data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
