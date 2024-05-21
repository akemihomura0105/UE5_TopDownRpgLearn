// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxHealth.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/Interface/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorCaptureDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	VigorCaptureDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorCaptureDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters AggregatorEvaluateParams;
	AggregatorEvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	AggregatorEvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorCaptureDef, Spec, AggregatorEvaluateParams, Vigor);

	TScriptInterface<ICombatInterface> SourceCombatInterface = Spec.GetEffectContext().GetInstigator();
	auto CombatLevel = SourceCombatInterface->GetCombatLevel();
	
	return 80.0f + 2.5f * Vigor + 10.f * CombatLevel;
}
