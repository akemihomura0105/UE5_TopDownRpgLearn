// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxMana.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/Interface/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	InteligenceCaptureDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	InteligenceCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	InteligenceCaptureDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(InteligenceCaptureDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters AggregatorEvaluateParams;
	AggregatorEvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	AggregatorEvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Inteligence = 0.f;
	GetCapturedAttributeMagnitude(InteligenceCaptureDef, Spec, AggregatorEvaluateParams, Inteligence);

	TScriptInterface<ICombatInterface> SourceCombatInterface = Spec.GetEffectContext().GetInstigator();
	auto CombatLevel = SourceCombatInterface->GetCombatLevel();

	return 50.0f + 2.5f * Inteligence + 15.f * static_cast<float>(CombatLevel);
}
