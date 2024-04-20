// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Aura/Player/AuraPlayerState.h"
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

void AAuraCharacter::InitAbilityActorInfo()
{
	auto PS = GetPlayerState<AAuraPlayerState>();
	check(PS);
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(this, PS);
	AttributeSet = PS->GetAttributeSet();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (GetLocalRole() == ROLE_Authority)
	{
		InitAbilityActorInfo();
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

UAbilitySystemComponent* AAuraCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
