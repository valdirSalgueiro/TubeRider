// Fill out your copyright notice in the Description page of Project Settings.

#include "TubeRiderGameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Engine/Public/TimerManager.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UTubeRiderGameInstance::UTubeRiderGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menu/IntroMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT("/Game/Menu/GameMenu"));
	if (!ensure(GameMenuBPClass.Class != nullptr)) return;
	GameMenuClass = GameMenuBPClass.Class;

	gameStarted = false;
	resetGame = false;
}

void UTubeRiderGameInstance::SetGameStarted(bool started)
{
	gameStarted = started;
}

bool UTubeRiderGameInstance::HasGameStarted()
{
	return gameStarted;
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
	return Menu;
}

void UTubeRiderGameInstance::LoadInGameMenu()
{
	if (!ensure(GameMenuClass != nullptr)) return;

	auto GameMenu = CreateWidget<UInGameMenu>(this, GameMenuClass);
	if (!ensure(GameMenu != nullptr)) return;
	GameMenu->Setup();
	GameMenu->SetMenuInterface(this);
}

void UTubeRiderGameInstance::LoadMainMenu()
{
	resetGame = true;
	Menu = LoadMenu();
	Menu->OpenMainMenu();
	Menu->FadeIn();
}
