// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rider.generated.h"

class USplineComponent;
class ATube;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TUBERIDER_API ARider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARider();

	UPROPERTY(EditAnywhere)
		ATube* tube;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UCameraComponent* Camera;

	float distance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
};
