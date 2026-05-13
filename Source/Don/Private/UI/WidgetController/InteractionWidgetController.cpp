#include "UI/WidgetController/InteractionWidgetController.h"

void UInteractionWidgetController::SetInteractionWidgetContext(const FInteractionWidgetContext& IWC)
{
	InteractionWidgetContext = IWC;
}

void UInteractionWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
}
