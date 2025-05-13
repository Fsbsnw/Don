// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class DON_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_HP;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_HP;
};
