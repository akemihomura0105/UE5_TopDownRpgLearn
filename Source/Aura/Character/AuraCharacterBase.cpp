// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AAuraCharacterBase::InitializeAttribute() const
{
	if (!HasAuthority())
	{
		return;
	}

	InitializePrimaryAttribute();
	InitializeSecondaryAttribute();
	InitializeVitalAttribute();
}

void AAuraCharacterBase::AddStartupAbilities() const
{
	if (auto AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraAbilitySystemComponent->AddAbilities(StartupAbilities);
	}
}

void AAuraCharacterBase::InitializePrimaryAttribute() const
{
	ApplyGameplayEffectToSelf(InitializePrimaryAttributeGameplayEffect, 1);
}

void AAuraCharacterBase::InitializeSecondaryAttribute() const
{
	ApplyGameplayEffectToSelf(InitializeSecondaryAttributeGameplayEffect, 1);
}

void AAuraCharacterBase::InitializeVitalAttribute() const
{
	ApplyGameplayEffectToSelf(InitializeVitalAttributeGameplayEffect, 1);
}

void AAuraCharacterBase::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GEClass, float Level) const
{
	auto ASC = GetAbilitySystemComponent();
	check(IsValid(ASC));
	check(GEClass);
	auto GEContext = ASC->MakeEffectContext();
	GEContext.AddSourceObject(this);
	auto GESpec = ASC->MakeOutgoingSpec(GEClass, 1, GEContext);
	ASC->ApplyGameplayEffectSpecToSelf(*GESpec.Data);
}
