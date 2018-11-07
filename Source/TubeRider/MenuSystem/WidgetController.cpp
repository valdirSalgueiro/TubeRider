#include "WidgetController.h"
#include "MenuSystem/MenuWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

WidgetController::WidgetController()
{
}

void WidgetController::Setup(UUserWidget* widget, UWorld* World)
{
	widget->AddToViewport();

	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(widget->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;
}

void WidgetController::Teardown(UUserWidget* widget, UWorld* World)
{
	widget->RemoveFromViewport();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}

