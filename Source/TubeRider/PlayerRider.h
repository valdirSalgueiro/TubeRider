// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Tube.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "PlayerRider.generated.h"

UCLASS()
class TUBERIDER_API APlayerRider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerRider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		ATube* tube;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UCameraComponent* Camera;
	USpringArmComponent* CameraSpringArm;
	float distance;
	FVector2D MovementInput;

	//Input functions
	void MoveRight(float AxisValue);
	float angle;
};
