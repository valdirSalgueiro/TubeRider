// Fill out your copyright notice in the Description page of Project Settings.

#include "Rider.h"
#include "Tube.h"
#include "Runtime/Engine/Classes/Camera/PlayerCameraManager.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARider::ARider()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	RootComponent = Camera;
	distance = 0;
}

// Called when the game starts or when spawned
void ARider::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	//Smoothly transition to our actor on begin play.
	OurPlayerController->SetViewTarget(this);
}

// Called every frame
void ARider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	distance += DeltaTime * 300.0f;

	if (tube != NULL)
	{
		auto SplineComponent = tube->GetSpline();
		int numberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();
		float totalLength = SplineComponent->GetSplineLength();
		if (distance >= totalLength / 2.0f)
		{
			tube->InsertNewPoints();
			numberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();
			totalLength = SplineComponent->GetSplineLength();
		}

		auto pos = SplineComponent->GetTransformAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
		pos.SetLocation(pos.GetLocation() + FVector(0, 15, 60));
		Camera->SetWorldTransform(pos);
	}
}

