// Fill out your copyright notice in the Description page of Project Settings.

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


ATube::ATube()
{	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = Spline;
	Spline->SetMobility(EComponentMobility::Type::Static);

	created = false;
	currentPoints = 0;
	PrimaryActorTick.bCanEverTick = true;
}

void ATube::BeginPlay()
{
	Super::BeginPlay();
}

void ATube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATube::InsertNewPoints()
{
	UE_LOG(LogTemp, Display, TEXT("inserting new new points"));
	int splinePointCount = Spline->GetNumberOfSplinePoints();
	FVector lastPoint = Spline->GetWorldLocationAtSplinePoint(splinePointCount - 1);
	for (int i = 0; i < 10; i++) {
		float randomY = FMath::FRandRange(-200, 200);
		float randomZ = FMath::FRandRange(-200, 200);
		UE_LOG(LogTemp, Display, TEXT("newpoint new %s"), *lastPoint.ToCompactString());
		lastPoint += FVector(100, randomY, randomZ);
		Spline->AddSplinePoint(lastPoint, ESplineCoordinateSpace::World);
		//lastPoint = newVector;
	}
	createSplineMesh();
}

void ATube::OnConstruction(const FTransform& Transform)
{
	if (!created)
	{
		this->createSplineMesh();
	}
}

void ATube::createSplineMesh()
{
	FVector locStart;
	FVector tanStart;
	FVector locEnd;
	FVector tanEnd;

	// Clean up stale mesh components
	//if (SplineMesh.Num() > 0)
	//{
	//	for (int32 i = 0; i < SplineMesh.Num(); i++)
	//	{
	//		if (SplineMesh[i])
	//		{
	//			SplineMesh[i]->DetachFromParent();
	//			SplineMesh[i]->DestroyComponent();
	//		}
	//	}
	//	SplineMesh.Empty();
	//}

	for (int32 i = currentPoints; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		USplineMeshComponent *splineMesh = NewObject<USplineMeshComponent>(this);
		splineMesh->RegisterComponent();
		UE_LOG(LogTemp, Display, TEXT("Added new USplineMeshComponent: %s"), *splineMesh->GetName());
		splineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		Spline->GetLocationAndTangentAtSplinePoint(i, locStart, tanStart, ESplineCoordinateSpace::Local);
		Spline->GetLocationAndTangentAtSplinePoint(i + 1, locEnd, tanEnd, ESplineCoordinateSpace::Local);
		UE_LOG(LogTemp, Display, TEXT("Start location and tangent: %s / %s"), *locStart.ToCompactString(), *tanStart.ToCompactString());
		UE_LOG(LogTemp, Display, TEXT("End location and tangent: %s / %s"), *locEnd.ToCompactString(), *tanEnd.ToCompactString());
		splineMesh->bSmoothInterpRollScale = true;
		splineMesh->SetMobility(EComponentMobility::Type::Movable);
		splineMesh->SetForwardAxis(ESplineMeshAxis::Y);
		splineMesh->SetStaticMesh(Mesh);
		splineMesh->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd);
		splineMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		splineMesh->AttachToComponent(Spline, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		SplineMesh.Add(splineMesh);
	}
	currentPoints = Spline->GetNumberOfSplinePoints() - 1;
	created = true;
}

