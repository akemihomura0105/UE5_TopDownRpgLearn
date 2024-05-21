// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"


// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyGameplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor))
	{
		check(GameplayEffectClass);
		auto EffectContext = TargetASC->MakeEffectContext();
		auto GameplayEffectSpec = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContext);
		auto ActiveGameplayEffect = TargetASC->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data, TargetASC);
		if (GameplayEffectSpec.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite)
		{
			auto& Effects = ActiveTargetToGameplayEffectMap.FindOrAdd(TargetASC);
			Effects.Add(ActiveGameplayEffect);
			UE_LOG(LogTemp, Log, TEXT("BeginOverlap, Add InfiniteEffect: %s"), *ActiveGameplayEffect.ToString());
		}
	}
}

void AAuraEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	if (InstantGameplayApplicationPolicy == EEffectApplicationPolicy::APPLY_ON_BEGIN_OVERLAP)
	{
		ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationGameplayApplicationPolicy == EEffectApplicationPolicy::APPLY_ON_BEGIN_OVERLAP)
	{
		ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteGameplayApplicationPolicy == EEffectApplicationPolicy::APPLY_ON_BEGIN_OVERLAP)
	{
		ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantGameplayApplicationPolicy == EEffectApplicationPolicy::APPLY_ON_END_OVERLAP)
	{
		ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationGameplayApplicationPolicy == EEffectApplicationPolicy::APPLY_ON_END_OVERLAP)
	{
		ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteGameplayRemovalPolicy == EEffectRemovalPolicy::REMOVE_ON_END_OVERLAP)
	{
		auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
		if (auto ActiveEffects = ActiveTargetToGameplayEffectMap.Find(TargetASC))
		{
			for (auto ActiveEffect : *ActiveEffects)
			{
				TargetASC->RemoveActiveGameplayEffect(ActiveEffect, 1);
				UE_LOG(LogTemp, Log, TEXT("EndOverlap, Remove Effect: %s"), *ActiveEffect.ToString());
			}
			ActiveTargetToGameplayEffectMap.Remove(TargetASC);
		}
	}
}
