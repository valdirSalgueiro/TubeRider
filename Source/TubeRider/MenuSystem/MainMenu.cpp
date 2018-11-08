// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UMG/Public/Animation/WidgetAnimation.h"

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
	
	//if (!ensure(HostButton != nullptr)) return false;
	//HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

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

//
//void UMainMenu::OpenJoinMenu()
//{
//	if (!ensure(MenuSwitcher != nullptr)) return;
//	if (!ensure(JoinMenu != nullptr)) return;
//	MenuSwitcher->SetActiveWidget(JoinMenu);
//}

//void UMainMenu::QuitPressed()
//{
//	UWorld* World = GetWorld();
//	if (!ensure(World != nullptr)) return;
//
//	APlayerController* PlayerController = World->GetFirstPlayerController();
//	if (!ensure(PlayerController != nullptr)) return;
//
//	PlayerController->ConsoleCommand("quit");
//}
