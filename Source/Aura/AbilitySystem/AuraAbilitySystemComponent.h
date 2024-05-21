// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectAppliedAssetTagsDelegate, const FGameplayTagContainer&)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void OnActorInfoSet();

	FOnEffectAppliedAssetTagsDelegate OnEffectAppliedAssetTags;
	
protected:
	//  [Server]
	void OnEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGE) const;
	
	UFUNCTION(Client, Reliable)
	void RPC_BroadCastAssetTagsWhenEffectApplied(const FGameplayTagContainer& AssetTags) const;
	
	

};
