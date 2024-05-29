// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTagsManager.h"

#include "GameplayTagsManager.h"


FAuraGameplayTagsManager& FAuraGameplayTagsManager::Get()
{
	static FAuraGameplayTagsManager Instance;
	return Instance;
}

void FAuraGameplayTagsManager::InitializeGameplayTags()
{
	auto& AuraGameplayTagsManager = FAuraGameplayTagsManager::Get();
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5103)
	// AuraGameplayTagsManager.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"));
	#define REGISTER_IMPL(NAME_ARG, DESC) AuraGameplayTagsManager.CONCAT(_, NAME_ARG) = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(APPLY(STRINGIFY,CONCAT(.,NAME_ARG))), FString(DESC));
    	REGISTER_GAMEPLAY_TAG
    #undef REGISTER_IMPL
#pragma warning(pop)
#endif
}
