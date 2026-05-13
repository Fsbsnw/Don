// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIManagerSubsystem.h"

#include "Data/UIConfigDataAsset.h"
#include "GameState/DonGameStateBase.h"
#include "Player/DonPlayerController.h"
#include "Player/DonPlayerState.h"
#include "UI/Widget/DonUserWidget.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "UI/WidgetController/InteractionWidgetController.h"

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();
	if (!World) return;

	World->GameStateSetEvent.AddUObject(this, &ThisClass::InitializeUIConfig);

	// 이미 GameState 존재하는 경우 대비
	if (AGameStateBase* GS = World->GetGameState())
	{
		InitializeUIConfig(GS);
	}
}

void UUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UUIManagerSubsystem::InitializeUIConfig(AGameStateBase* GameState)
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	ADonGameStateBase* GS = Cast<ADonGameStateBase>(GameState);
	if (GS && GS->UIConfigDataAsset)
	{
		this->ConfigData = GS->UIConfigDataAsset->UIConfigData.WidgetMap;
		ActiveWidgets.Empty();
	}
}

void UUIManagerSubsystem::InitializeUIBinding(APlayerController* PC)
{
	if (ADonPlayerController* LPC = Cast<ADonPlayerController>(PC))
	{
		LPC->OnWidgetToggleRequested.RemoveAll(this);
		LPC->OnWidgetToggleRequested.AddUObject(this, &ThisClass::OnToggleRequested);

		LPC->OnInteractionWidgetRequested.RemoveAll(this);
		LPC->OnInteractionWidgetRequested.AddUObject(this, &UUIManagerSubsystem::OpenInteractionWidget);
	}
}

void UUIManagerSubsystem::OnToggleRequested(FGameplayTag InputTag)
{
	ToggleWidget(InputTag);
}

void UUIManagerSubsystem::ToggleWidget(FGameplayTag Tag)
{
	// 이미 활성화된 위젯인 경우
	if (UDonUserWidget** FoundWidget = ActiveWidgets.Find(Tag))
	{
		UDonUserWidget* Widget = *FoundWidget;
		// 이미 위젯이 뷰포트에 보이고 있는 경우 -> 비활성화
		if (Widget->IsVisible())
		{
			const EWidgetCachePolicy Policy = ConfigData[Tag].CachePolicy;
			// 자주 사용하지 않는 위젯은 캐싱 X
			if (Policy == EWidgetCachePolicy::DestroyOnClose)
			{
				Widget->CloseWidgetAndChildren();
				ActiveWidgets.Remove(Tag);
			}
			// 자주 사용하는 위젯의 경우 캐싱 O
			else
			{
				Widget->SetVisibility(ESlateVisibility::Collapsed);
				Widget->ClearAllChildren();
			}
		}
		// 위젯이 뷰포트에 보이지 않고 있는 경우 -> 캐싱된 위젯은 다시 보여주기
		else
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	// 처음 위젯을 호출하는 경우
	else
	{
		OpenWidget(Tag);
	}
}

void UUIManagerSubsystem::OpenWidget(FGameplayTag Tag)
{
	if (!ConfigData.Contains(Tag) || !ConfigData[Tag].WidgetClass) return;

	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !PC->IsLocalController()) return;

	// 위젯 생성
	UUserWidget* BaseWidget = CreateWidget<UUserWidget>(PC, ConfigData[Tag].WidgetClass);
	if (BaseWidget == nullptr) return;

	// 캐스팅
	UDonUserWidget* Widget = Cast<UDonUserWidget>(BaseWidget);
	if (Widget == nullptr) return;

	// 위젯 기본 세팅
	if (ConfigData[Tag].WidgetControllerClass != nullptr)
	{
		UDonWidgetController* WidgetController = NewObject<UDonWidgetController>(PC, ConfigData[Tag].WidgetControllerClass);
		
		ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
		
		WidgetController->SetWidgetControllerParams(WCParams);
		WidgetController->BindCallbacksToDependencies();
		Widget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValues();
	}
	Widget->AddToViewport();
	ActiveWidgets.Add(Tag, Widget);
}

void UUIManagerSubsystem::OpenInteractionWidget(FGameplayTag Tag, const FInteractionWidgetContext& IWC)
{
	ToggleWidget(Tag);

	// 상호작용 위젯인 경우, InteractionWidgetContext 설정 추가
	if (UDonUserWidget** Found = ActiveWidgets.Find(Tag))
	{
		UDonUserWidget* Widget = *Found;
		if (UInteractionWidgetController* IWController = Cast<UInteractionWidgetController>(Widget->WidgetController))
		{
			IWController->SetInteractionWidgetContext(IWC);
		}
	}
}
