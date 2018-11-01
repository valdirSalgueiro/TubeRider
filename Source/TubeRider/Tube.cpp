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
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include <time.h>



ATube::ATube()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetGenerateOverlapEvents(false);
	Spline->SetMobility(EComponentMobility::Type::Movable);
	RootComponent = Spline;

	//Spline->SetCollisionProfileName(FName("OverlapAll"));
	Spline->SetGenerateOverlapEvents(false);

	created = false;
	currentPoints = 0;
	PrimaryActorTick.bCanEverTick = true;
	srand(time(NULL));

	SetActorEnableCollision(false);

	playerDistance = 0;

	removePointIndex = 0;
}

void ATube::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector::ZeroVector);
	lastPoint = GetActorLocation();
	int splinePointCount = Spline->GetNumberOfSplinePoints();
	while (splinePointCount < 1000)
	{
		InsertNewPoints(0, false);
		splinePointCount = Spline->GetNumberOfSplinePoints();
	}
	isReady = true;
}

void ATube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATube::InsertNewPoints(float distance, bool createMesh)
{
	playerDistance = distance;
	int splinePointCount = Spline->GetNumberOfSplinePoints();
	int i = 0;

	//UE_LOG(LogTemp, Display, TEXT("%d %d"), splinePointCount - 2, splinePointCount - 2 % 20);
	if ((splinePointCount - 2) % 50 == 0) {
		int randomDirection = rand() % 2;
		angleHDest = GetNewRandomAngle();
		angleVDest = GetNewRandomAngle();
		angleHDest = randomDirection ? angleHDest : -angleHDest;
		randomDirection = rand() % 2;
		angleVDest = randomDirection ? angleVDest : -angleVDest;
	}
	float angleHVar = 1;
	float angleVVar = 1;

	//angleHDest = 90;
	//angleVDest = 0;

	if (angleH < angleHDest) {
		angleH += angleHVar;
	}
	else if (angleH > angleHDest) {
		angleH -= angleHVar;
	}

	if (angleV < angleVDest) {
		angleV += angleVVar;
	}
	else if (angleV > angleVDest) {
		angleV -= angleVVar;
	}

	//angleH = FMath::Max(0.f, angleH);
	//angleH = FMath::Min(180.f, angleH);
	//angleV = FMath::Max(0.f, angleV);
	//angleV = FMath::Min(180.f, angleV);
	//UE_LOG(LogTemp, Display, TEXT("%f %f"), angleH, angleV);

	float newX = FMath::Cos(FMath::DegreesToRadians(angleH)) * 7.0f;
	float newY = FMath::Sin(FMath::DegreesToRadians(angleH)) * 7.0f;
	float newZ = FMath::Sin(FMath::DegreesToRadians(angleV)) * 7.0f;
	lastPoint += FVector(newX, newY, newZ);
	i++;
	Spline->AddSplinePoint(lastPoint, ESplineCoordinateSpace::World);

	if (createMesh)
		createSplineMesh();
}

int ATube::GetNewRandomAngle()
{
	int random = rand() % 5;
	float angle = 0;
	switch (random) {
	case 0:
		angle = 0;
		break;
	case 1:
		angle = 45;
		break;
	case 2:
		angle = 90;
		break;
	case 3:
		angle = 135;
		break;
	default:
		angle = 180;
		break;
	}
	return angle;
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

	if (Spline->GetDistanceAlongSplineAtSplinePoint(removePointIndex) < playerDistance && SplineMesh.Num() > 0 && SplineMesh[0])
	{
		UE_LOG(LogTemp, Display, TEXT("in cleaning up %d %d %f %f"), SplineMesh.Num(), removePointIndex, Spline->GetDistanceAlongSplineAtSplinePoint(removePointIndex), playerDistance);
		SplineMesh[0]->UnregisterComponent();
		SplineMesh[0]->DetachFromParent();
		SplineMesh[0]->DestroyComponent();
		SplineMesh.RemoveAt(0);
		removePointIndex++;
		if (removePointIndex % 50 == 0 && ObstaclesActor.Num() > 0) {
			if (ObstaclesActor[0])
			{
				ObstaclesActor[0]->Destroy();
				ObstaclesActor.RemoveAt(0);
			}
		}
	}

	for (int32 i = currentPoints; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		USplineMeshComponent *splineMesh = NewObject<USplineMeshComponent>(this);
		splineMesh->RegisterComponent();
		splineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		Spline->GetLocationAndTangentAtSplinePoint(i, locStart, tanStart, ESplineCoordinateSpace::Local);
		Spline->GetLocationAndTangentAtSplinePoint(i + 1, locEnd, tanEnd, ESplineCoordinateSpace::Local);
		splineMesh->bSmoothInterpRollScale = true;
		splineMesh->SetMobility(EComponentMobility::Type::Movable);
		splineMesh->SetForwardAxis(ESplineMeshAxis::Y);
		splineMesh->SetStaticMesh(Mesh);
		splineMesh->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd);
		splineMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		splineMesh->AttachToComponent(Spline, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		splineMesh->SetStartOffset(FVector2D(-15, 0));
		splineMesh->SetEndOffset(FVector2D(-15, 0));
		SplineMesh.Add(splineMesh);
		if (i != 0 && i % 50 == 0)
		{
			UWorld* const World = GetWorld();
			if (World && Obstacles.Num() > 0)
			{
				auto transform = Spline->GetTransformAtSplinePoint(i, ESplineCoordinateSpace::World);
				auto rotation = FRotator(transform.GetRotation()).Add(90, 0, 0);
				auto actor = World->SpawnActor<AObstacle>(Obstacles[0], transform.GetLocation(), rotation);
				ObstaclesActor.Add(actor);
			}
		}
	}

	currentPoints = Spline->GetNumberOfSplinePoints() - 1;
	created = true;
}
