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

	Spline->SetGenerateOverlapEvents(false);

	currentPoint = 0;
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(false);
	removePointIndex = 0;
	elapsedSeconds = 0;
	initializationSize = 50;
	currentSplineMesh = 0;

	angleHDest = 0;
	angleVDest = 0;

	angleH = 0;
	angleV = 0;


	if (spawner == NULL) {
		spawner = new ObstacleSpawner();
	}	
}

void ATube::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	if (Mesh && Material) {
		Mesh->SetMaterial(0, Material);
	}
	Spline->ClearSplinePoints();
	SetActorLocation(FVector::ZeroVector);
	lastPoint = GetActorLocation();
	int splinePointCount = Spline->GetNumberOfSplinePoints();
	while (splinePointCount < initializationSize)
	{
		USplineMeshComponent *splineMesh = NewObject<USplineMeshComponent>(this);
		splineMesh->RegisterComponent();
		splineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		splineMesh->bSmoothInterpRollScale = true;
		splineMesh->SetMobility(EComponentMobility::Type::Movable);
		splineMesh->SetForwardAxis(ESplineMeshAxis::Y);
		splineMesh->SetStaticMesh(Mesh);
		splineMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		splineMesh->AttachToComponent(Spline, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		splineMesh->SetStartOffset(FVector2D(-15, 0));
		splineMesh->SetEndOffset(FVector2D(-15, 0));
		SplineMesh.Add(splineMesh);
		InsertNewPoints(0);
		splinePointCount = Spline->GetNumberOfSplinePoints();
	}
	CreateSplineMesh(false);
	isReady = true;
}

void ATube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	elapsedSeconds += DeltaTime;
}

void ATube::GetNewAngle()
{
	GetHorizontalAngle();
	GetVerticalAngle();
}

void ATube::GetHorizontalAngle()
{
	int randomDirection;
	angleHDest = GetNewRandomAngle();
	if (lastPoint.Y < 0 && angleHDest < 0) {
		angleHDest = -angleHDest;
	}
	else if (lastPoint.Y > 500 && angleHDest > 0) {
		angleHDest = -angleHDest;
	}
	else if (lastPoint.Y > 0 && lastPoint.Y < 500) {
		randomDirection = FMath::Rand() % 2;
		angleHDest = randomDirection ? angleHDest : -angleHDest;
	}
}

void ATube::GetVerticalAngle()
{
	int randomDirection;
	angleVDest = GetNewRandomAngle();

	if (lastPoint.Z < 0 && angleVDest < 0) {
		angleVDest = -angleVDest;
	}
	else if (lastPoint.Z > 200 && angleVDest > 0) {
		angleVDest = -angleVDest;
	}
	else if (lastPoint.Z > 0 && lastPoint.Z < 200)
	{
		randomDirection = FMath::Rand() % 2;
		angleVDest = randomDirection ? angleVDest : -angleVDest;
	}
}

void ATube::InsertNewPoints(float distance)
{
	float angleHVar = 1.5f;
	float angleVVar = 1.5f;

	if (angleH < angleHDest) {
		angleH += angleHVar;
	}
	else if (angleH > angleHDest) {
		angleH -= angleHVar;
	}
	else
	{
		GetHorizontalAngle();
	}

	if (angleV < angleVDest) {
		angleV += angleVVar;
	}
	else if (angleV > angleVDest) {
		angleV -= angleVVar;
	}
	else
	{
		GetVerticalAngle();
	}/*

	if (angleH == angleHDest && angleV == angleVDest)
	{
		GetNewAngle();
	}*/
	float newX = FMath::Cos(FMath::DegreesToRadians(angleH)) * 7.0f;
	float newY = FMath::Sin(FMath::DegreesToRadians(angleH)) * 7.0f;
	float newZ = FMath::Sin(FMath::DegreesToRadians(angleV)) * 7.0f;
	lastPoint += FVector(newX, newY, newZ);

	Spline->AddSplinePoint(lastPoint, ESplineCoordinateSpace::World);
}

int ATube::GetNewRandomAngle()
{
	int random = FMath::Rand() % 6;
	float angle = 0;
	switch (random) {
	case 0:
		angle = 0;
		break;
	case 1:
		angle = 15;
		break;
	case 2:
		angle = 30;
		break;
	case 3:
		angle = 45;
		break;
	case 4:
		angle = 60;
		break;
	case 5:
		angle = 75;
		break;
	case 6:
		angle = 89;
		break;
	case 7:
		angle = 270;
		break;
	case 8:
		angle = 315;
		break;
	case 9:
		angle = 360;
		break;
	}
	return angle;
}

void ATube::CreateSplineMesh(bool remove)
{
	FVector locStart;
	FVector tanStart;
	FVector locEnd;
	FVector tanEnd;



	if (remove) {
		float removedSplineDistance = Spline->GetDistanceAlongSplineAtSplinePoint(0);
		Spline->RemoveSplinePoint(0, false);
		if (ObstaclesActor.Num() > 0) {
			if (ObstaclesActor[0].obstacle && ObstaclesActor[0].distance < removedSplineDistance)
			{
				ObstaclesActor[0].obstacle->Destroy();
				ObstaclesActor.RemoveAt(0);
			}
		}
		currentPoint--;
	}

	for (; currentPoint < Spline->GetNumberOfSplinePoints() - 1; currentPoint++)
	{
		Spline->GetLocationAndTangentAtSplinePoint(currentPoint, locStart, tanStart, ESplineCoordinateSpace::Local);
		Spline->GetLocationAndTangentAtSplinePoint(currentPoint + 1, locEnd, tanEnd, ESplineCoordinateSpace::Local);

		if (currentSplineMesh >= SplineMesh.Num())
			currentSplineMesh = 0;
		auto splineMesh = SplineMesh[currentSplineMesh++];
		splineMesh->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd);
		splineMesh->SetStartOffset(FVector2D(-15, 0));
		splineMesh->SetEndOffset(FVector2D(-15, 0));
	}

	auto obstacle = spawner->SpawnObject(world, Spline, Obstacles, currentPoint, elapsedSeconds);
	if (obstacle)
	{
		float distance = Spline->GetDistanceAlongSplineAtSplinePoint(currentPoint);
		ObstacleDistance newObstacle;
		newObstacle.distance = distance;
		newObstacle.obstacle = obstacle;
		ObstaclesActor.Add(newObstacle);
	}
}
