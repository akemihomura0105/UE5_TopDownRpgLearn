// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Ztool/MConcat.h"
#define REGISTER_GAMEPLAY_TAG\
	REGISTER_IMPL(VA_LIST(Attributes, Vital, Health), "生命值")\
	REGISTER_IMPL(VA_LIST(Attributes, Vital, Mana), "魔法值")\
	REGISTER_IMPL(VA_LIST(Attributes, Primary, Strength),)\
	REGISTER_IMPL(VA_LIST(Attributes, Primary, Intelligence),)\
	REGISTER_IMPL(VA_LIST(Attributes, Primary, Resilience),)\
	REGISTER_IMPL(VA_LIST(Attributes, Primary, Vigor),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, Armor),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, ArmorPenetration),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, BlockChance),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, CriticalHitChance),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, CriticalHitDamage),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, CriticalHitResistance),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, HealthRegeneration),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, ManaRegeneration),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, MaxHealth),)\
	REGISTER_IMPL(VA_LIST(Attributes, Secondary, MaxMana),)\
	REGISTER_IMPL(VA_LIST(InputTag, LMB),)\
	REGISTER_IMPL(VA_LIST(InputTag, RMB),)\
	REGISTER_IMPL(VA_LIST(InputTag, 1),)\
	REGISTER_IMPL(VA_LIST(InputTag, 2),)\
	REGISTER_IMPL(VA_LIST(InputTag, 3),)\
	REGISTER_IMPL(VA_LIST(InputTag, 4),)

class AURA_API FAuraGameplayTagsManager
{
public:
	static FAuraGameplayTagsManager& Get();
	static void InitializeGameplayTags();

#define REGISTER_IMPL(NAME_ARG, DESC) FGameplayTag CONCAT(_, NAME_ARG);
	// FGameplayTag Attributes_Vital_Health;
	REGISTER_GAMEPLAY_TAG
#undef REGISTER_IMPL

private:
	FAuraGameplayTagsManager()
	{
	}
};
