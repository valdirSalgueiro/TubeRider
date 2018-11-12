// Fill out your copyright notice in the Description page of Project Settings.

#include "TubePlayerController.h"
#include "Engine/GameInstance.h"
#include "TubeRiderGameInstance.h"

void ATubePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATubePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Cancel", IE_Pressed, this, &ATubePlayerController::Cancel);
}

void ATubePlayerController::Cancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Cancel pressed"));
	auto gameInstance = Cast<UTubeRiderGameInstance>(GetGameInstance());
	gameInstance->SetGameStarted(false);
	gameInstance->LoadInGameMenu();
}