// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "DonAbilityTypes.h"
#include "DonGameplayTags.h"
#include "AbilitySystem/DonAttributeSet.h"

struct AttributeStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vigor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DodgeChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	AttributeStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDonAttributeSet, Vigor, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDonAttributeSet, DodgeChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDonAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDonAttributeSet, CriticalHitDamage, Source, false);
	}
};

static const AttributeStatics& GetAttributeStatics()
{
	static AttributeStatics Statics;
	return Statics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetAttributeStatics().VigorDef);
	RelevantAttributesToCapture.Add(GetAttributeStatics().DodgeChanceDef);
	RelevantAttributesToCapture.Add(GetAttributeStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetAttributeStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FDonGameplayTags& Tags = FDonGameplayTags::Get();
	FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	float IncomingDamage = Spec.GetSetByCallerMagnitude(Tags.Damage, false);


	float SourceVigor = 0.f;
	float TargetXPRate = 0.f;
	float CriticalHitChance = 0.f;
	float CriticalHitDamage = 0.f;

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		FAggregatorEvaluateParameters EvaluateParameters;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().VigorDef, EvaluateParameters, SourceVigor);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().DodgeChanceDef, EvaluateParameters, TargetXPRate);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().CriticalHitChanceDef, EvaluateParameters, CriticalHitChance);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().CriticalHitDamageDef, EvaluateParameters, CriticalHitDamage);
	}

	const bool bCriticalHit = FMath::FRandRange(0.f, 100.f) < CriticalHitChance;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	if (EffectContextHandle.IsValid() && EffectContextHandle.Get())
	{
		if (FDonGameplayEffectContext* DonContext = static_cast<FDonGameplayEffectContext*>(EffectContextHandle.Get()))
		{
			DonContext->SetIsCriticalHit(bCriticalHit);
		}
	}

	float FinalDamage = 0.f;
	
	if (bCriticalHit)
	{
		FinalDamage = (IncomingDamage + (SourceVigor * 2)) * 2 * ((100 + CriticalHitDamage) / 100);
		UE_LOG(LogTemp, Warning, TEXT("Critical Hit"));
	}
	else
	{
		FinalDamage = IncomingDamage + (SourceVigor * 2);
		UE_LOG(LogTemp, Warning, TEXT("Normal Hit"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Incoming Damage : %f, Final Damage : %f"), IncomingDamage, FinalDamage);
	
	FGameplayModifierEvaluatedData EvaluatedData(
		UDonAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Override,
		FinalDamage
	);
	
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
