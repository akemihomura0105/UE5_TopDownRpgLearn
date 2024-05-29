// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemComponent.h"

#include "Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::OnActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnEffectApplied);
}

void UAuraAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGE) const
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
	FGameplayTagContainer AssetTags;
	GESpec.GetAllAssetTags(AssetTags);
	RPC_BroadCastAssetTagsWhenEffectApplied(AssetTags);
}

void UAuraAbilitySystemComponent::AddAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilityClasses)
{
	for (const auto& AbilityClass : AbilityClasses)
	{
		FGameplayAbilitySpec GameplayAbilitySpec(AbilityClass, 1);
		if (auto AuraGameplayAbility = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartupInputTag);
		}
		GiveAbility(GameplayAbilitySpec);
	}
}

void UAuraAbilitySystemComponent::OnAbilityInputPressed(FGameplayTag InputTag)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, "is auth");
	}
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

			// make sure the ActorInfo and then Actor on that FGameplayAbilityActorInfo are valid, if not bail out.
			if (ActorInfo == nullptr || !ActorInfo->OwnerActor.IsValid() || !ActorInfo->AvatarActor.IsValid())
			{
				return;
			}


			const ENetRole NetMode = ActorInfo->AvatarActor->GetLocalRole();

			// This should only come from button presses/local instigation (AI, etc).
			if (NetMode == ROLE_SimulatedProxy)
			{
				return;
			}
			AbilitySpecInputPressed(AbilitySpec);
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::OnAbilityInputHeld(FGameplayTag InputTag)
{
}

void UAuraAbilitySystemComponent::OnAbilityInputReleased(FGameplayTag InputTag)
{
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::RPC_BroadCastAssetTagsWhenEffectApplied_Implementation(const FGameplayTagContainer& AssetTags) const
{
	OnEffectAppliedAssetTags.Broadcast(AssetTags);
}
