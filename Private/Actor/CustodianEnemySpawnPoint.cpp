// Copyright Sertim all rights reserved


#include "Actor/CustodianEnemySpawnPoint.h"
#include "Character/CustodianEnemy.h"

void ACustodianEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACustodianEnemy* Enemy = GetWorld()->SpawnActorDeferred<ACustodianEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
