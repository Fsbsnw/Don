// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_GoldGain.h"

#include "DonGameplayTags.h"
#include "AbilitySystem/DonAttributeSet.h"

struct MoneyGainRateStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MoneyGainRate);

	MoneyGainRateStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDonAttributeSet, MoneyGainRate, Target, false);
	}
};

static const MoneyGainRateStatics& GetMoneyGainRateStatics()
{
	static MoneyGainRateStatics Statics;
	return Statics;
}

void UExecCalc_GoldGain::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FDonGameplayTags& Tags = FDonGameplayTags::Get();
	float MoneyGainRate = 0.f;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayEffectContextHandle& Context = Spec.GetContext();

	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	if (TargetASC)
	{
		FAggregatorEvaluateParameters EvaluateParameters;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMoneyGainRateStatics().MoneyGainRateDef, EvaluateParameters, MoneyGainRate);

		UE_LOG(LogTemp, Log, TEXT("Captured MoneyGainRate: %f"), MoneyGainRate);
		MoneyGainRate = FMath::Max(0.f, MoneyGainRate);
	}

	float BaseGoldAmount = Spec.GetSetByCallerMagnitude(Tags.Attributes_Secondary_MoneyGainRate, false);
	float FinalGoldAmount = BaseGoldAmount * (1.f + MoneyGainRate);

	FGameplayModifierEvaluatedData EvaluatedData(
		UDonAttributeSet::GetMoneyGainRateAttribute(),
		EGameplayModOp::Additive,
		FinalGoldAmount
	);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
