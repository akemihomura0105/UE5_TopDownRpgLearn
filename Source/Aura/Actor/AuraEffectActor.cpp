// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"


// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetupAttachment(MeshComponent);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnBeginOverlapped);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnEndOverlapped);
}

void AAuraEffectActor::OnBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto AscInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		auto AuraAttributeSet = const_cast<UAuraAttributeSet*>(Cast<UAuraAttributeSet>(AscInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass())));
		AuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.0f);
		Destroy();
	}
}

void AAuraEffectActor::OnEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
