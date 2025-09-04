// Copyright Sertim all rights reserved


#include "AbilitySystem/Data/AttributeInfo.h"

FCustodianAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (FCustodianAttributeInfo Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("CANT FIND INFO FOR ATTRIBUTE TAG [%s] on ATTRIBUTEINFO [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FCustodianAttributeInfo();
}
