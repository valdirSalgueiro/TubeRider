// Fill out your copyright notice in the Description page of Project Settings.

#include "TubeRiderGameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Engine/Public/TimerManager.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UTubeRiderGameInstance::UTubeRiderGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menu/IntroMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;
	controller = new WidgetController();

	currentScreen = 0;
}

void UTubeRiderGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

UMainMenu* UTubeRiderGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return nullptr;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return nullptr;
	Menu->Setup();
	Menu->SetMenuInterface(this);

	//controller->Setup(Menu, GetWorld());
	return Menu;
}

void UTubeRiderGameInstance::NextScreen()
{
	currentScreen++;
	if (!ensure(Menu != nullptr)) return;

	//controller->Setup(Menu, GetWorld());
}