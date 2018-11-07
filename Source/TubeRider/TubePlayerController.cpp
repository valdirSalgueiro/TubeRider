// Fill out your copyright notice in the Description page of Project Settings.

#include "TubePlayerController.h"

void ATubePlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}