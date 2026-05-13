#pragma once

#include "CoreMinimal.h"
#include "DonWidgetController.h"
#include "Character/Interface/InteractInterface.h"
#include "InteractionWidgetController.generated.h"

UCLASS()
class UInteractionWidgetController : public UDonWidgetController
{
	GENERATED_BODY()
public:
	void SetInteractionWidgetContext(const FInteractionWidgetContext& IWC);

	virtual void BroadcastInitialValues() override;

private:
	FInteractionWidgetContext InteractionWidgetContext;
};
