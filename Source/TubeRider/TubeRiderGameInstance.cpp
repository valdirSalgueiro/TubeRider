// Fill out your copyright notice in the Description page of Project Settings.

#include "TubeRiderGameInstance.h"

#include "UObject/ConstructorHelpers.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UTubeRiderGameInstance::UTubeRiderGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menu/Intro"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;
	controller = new WidgetController();
}

void UTubeRiderGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UTubeRiderGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	controller->Setup(Menu, GetWorld());

	//controller->SetMenuInterface(this);
}