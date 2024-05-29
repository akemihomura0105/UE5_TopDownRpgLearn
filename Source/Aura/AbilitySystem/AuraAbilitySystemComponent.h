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
	void AddAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilityClasses);

	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputHeld(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);

protected:
	//  [Server]
	void OnEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGE) const;


	UFUNCTION(Client, Reliable)
	void RPC_BroadCastAssetTagsWhenEffectApplied(const FGameplayTagContainer& AssetTags) const;
};
