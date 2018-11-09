// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "TubeRiderGameInstance.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	UProperty* prop = GetClass()->PropertyLink;
	animations.Empty();

	// Run through all properties of this class to find any widget animations
	while (prop != nullptr)
	{
		// Only interested in object properties
		if (prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* objectProp = Cast<UObjectProperty>(prop);

			// Only want the properties that are widget animations
			if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* object = objectProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* widgetAnim = Cast<UWidgetAnimation>(object);

				if (widgetAnim != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Found animation %s"), *prop->GetFName().ToString());
					animations.Add(prop->GetFName(), widgetAnim);
				}
			}
		}

		prop = prop->PropertyLinkNext;
	}
	
	if (!ensure(Quit != nullptr)) return false;
	Quit->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	if (!ensure(Start != nullptr)) return false;
	Start->OnClicked.AddDynamic(this, &UMainMenu::StartGamePressed);

	return true;
}
void UMainMenu::FadeIn() {
	PlayAnimation(animations[FName("FadeIn")]);
}

void UMainMenu::FadeOut() {
	PlayAnimation(animations[FName("FadeOut")]);
}

void UMainMenu::SetText(const FText& text) {
	TextIntro->SetText(text);
}


void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::StartGamePressed()
{
	Teardown();
	Cast<UTubeRiderGameInstance>(GetGameInstance())->SetGameStarted(true);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
