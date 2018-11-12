// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
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

	void SetGameStarted(bool started);

	bool HasGameStarted();

	bool HasToReset() {
		return resetGame;
	}

	void HasReset() {
		resetGame = false;
	}

	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
		UMainMenu* LoadMenu();

	void LoadInGameMenu();

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> GameMenuClass;

	class UMainMenu* Menu;

	bool gameStarted;

	bool resetGame;
};
