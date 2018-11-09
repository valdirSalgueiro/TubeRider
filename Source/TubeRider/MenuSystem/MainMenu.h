// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class TUBERIDER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetText(const FText& text);
	UFUNCTION(BlueprintCallable)
	void FadeIn();
	UFUNCTION(BlueprintCallable)
	void FadeOut();
	UFUNCTION()
	void OpenMainMenu();



protected:
	virtual bool Initialize();


private:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Screens;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextIntro;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* Start;

	UPROPERTY(meta = (BindWidget))
	class UButton* Quit;

	TMap<FName, UWidgetAnimation*> animations;

	UFUNCTION()
	void QuitPressed();

	UFUNCTION()
	void StartGamePressed();

};
