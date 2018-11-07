// Fill out your copyright notice in the Description page of Project Settings.

#include "TubeRiderGameModeBase.h"

void ATubeRiderGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void ATubeRiderGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{	
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}


