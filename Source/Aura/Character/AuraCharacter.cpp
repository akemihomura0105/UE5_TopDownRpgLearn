// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Player/AuraPlayerState.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

int32 AAuraCharacter::GetCombatLevel()
{
	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetCombatLevel();
}

FVector AAuraCharacter::GetCombatProjectileGenerateLocation()
{
	check(Weapon);
	return Weapon->GetSocketByName(WeaponTipName)->GetSocketLocation(Weapon);
}

void AAuraCharacter::InitAbilityActorInfo()
{
	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->OnActorInfoSet();
	AttributeSet = AuraPlayerState->GetAttributeSet();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		InitAbilityActorInfo();
		InitializeAttribute();
		AddStartupAbilities();
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	auto NetMode = AuraPlayerState->GetLocalRole();
	InitAbilityActorInfo();
}

UAbilitySystemComponent* AAuraCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
