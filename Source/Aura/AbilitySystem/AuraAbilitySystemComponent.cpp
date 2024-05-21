// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::OnActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnEffectApplied);
}

void UAuraAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGE) const
{
	FGameplayTagContainer AssetTags;
	GESpec.GetAllAssetTags(AssetTags);
	if (GetOwnerRole() == ROLE_Authority)
	{
		RPC_BroadCastAssetTagsWhenEffectApplied(AssetTags);
	}
}

void UAuraAbilitySystemComponent::RPC_BroadCastAssetTagsWhenEffectApplied_Implementation(const FGameplayTagContainer& AssetTags) const
{
	OnEffectAppliedAssetTags.Broadcast(AssetTags);
}
