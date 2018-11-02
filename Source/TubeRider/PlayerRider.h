// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Tube.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/SpotLightComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" 
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Camera/CameraShake.h"
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


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void Shake(float scale);

protected:

	float distance;
	FVector2D MovementInput;

	//Input functions
	void MoveRight(float AxisValue);
	float angle;

	UPROPERTY(EditAnywhere, Category = "Player")
		TSubclassOf<UCameraShake> cameraShake;

private:
	UPROPERTY(EditAnywhere, Category = "Player", meta = (AllowPrivateAccess = "true"))
	ATube* tube;

	UPROPERTY(EditAnywhere, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float playerVelocity;

	UPROPERTY(EditAnywhere, Category = "Player", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = "Player", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Player", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere, Category = "Player", meta = (AllowPrivateAccess = "true"))
		USpotLightComponent* FlashLightComponent;



};
