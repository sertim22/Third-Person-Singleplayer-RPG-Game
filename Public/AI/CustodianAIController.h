// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CustodianAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

// Custom AI controller class that must be used by all AI characters in game
UCLASS()
class CUSTODIAN_API ACustodianAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ACustodianAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
