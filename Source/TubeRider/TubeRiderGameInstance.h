// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "MenuSystem/WidgetController.h"
#include "TubeRiderGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TUBERIDER_API UTubeRiderGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()


public:
	UTubeRiderGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
		void LoadMenu();

private:
	TSubclassOf<class UUserWidget> MenuClass;
	//TSubclassOf<class UUserWidget> InGameMenuClass;
	WidgetController* controller;

	class UUserWidget* Menu;

	
};
