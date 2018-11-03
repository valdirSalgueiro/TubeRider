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
	playerDistance = 0;
	removePointIndex = 0;
	insertedPoints = 0;
	initializationSize = 1000;
	currentSplineMesh = 0;
}

void ATube::BeginPlay()
{
	Super::BeginPlay();
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
}

void ATube::InsertNewPoints(float distance)
{
	playerDistance = distance;
	int i = 0;

	insertedPoints++;
	if (insertedPoints % 50 == 0) {
		angleHDest = GetNewRandomAngle();
		angleVDest = GetNewRandomAngle();
		int randomDirection;
		if (lastPoint.Y < 0 && angleHDest < 0) {
			UE_LOG(LogTemp, Display, TEXT("lastPoint.Y < 0 angleHDest < 0 %f %f"), lastPoint.Y, angleHDest);
			angleHDest = -angleHDest;
		}
		else if (lastPoint.Y > 500 && angleHDest > 0) {
			UE_LOG(LogTemp, Display, TEXT("lastPoint.Y > 500 angleHDest > 0 %f %f"), lastPoint.Y, angleHDest);
			angleHDest = -angleHDest;
		}
		else if (lastPoint.Y > 0 && lastPoint.Y < 500) {
			UE_LOG(LogTemp, Display, TEXT("else %f %f"), lastPoint.Y, angleHDest);
			randomDirection = FMath::Rand() % 2;
			angleHDest = randomDirection ? angleHDest : -angleHDest;
		}
		
		if (lastPoint.Z < 0 && angleVDest < 0) {
			//UE_LOG(LogTemp, Display, TEXT("lastPoint.Z < 0 angleVDest < 0 %f %f"), lastPoint.Z, angleVDest);
			angleVDest = -angleVDest;
		}
		else if (lastPoint.Z > 200 && angleVDest > 0) {
			//UE_LOG(LogTemp, Display, TEXT("lastPoint.Z > 200 angleVDest > 0 %f %f"), lastPoint.Z, angleVDest);
			angleVDest = -angleVDest;
		}
		else if(lastPoint.Z > 0 && lastPoint.Z < 200)
		{
			//UE_LOG(LogTemp, Display, TEXT("else %f %f"), lastPoint.Z, angleVDest);
			randomDirection = FMath::Rand() % 2;
			angleVDest = randomDirection ? angleVDest : -angleVDest;
		}
	}

	float angleHVar = 1;
	float angleVVar = 1;

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

	//angleHDest = 45;

	float newX = FMath::Cos(FMath::DegreesToRadians(angleH)) * 7.0f;
	float newY = FMath::Sin(FMath::DegreesToRadians(angleH)) * 7.0f;
	float newZ = FMath::Sin(FMath::DegreesToRadians(angleV)) * 7.0f;
	lastPoint += FVector(newX, newY, newZ);

	i++;
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
		angle = 270;
		break;
	case 7:
		angle = 315;
		break;
	case 8:
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

	//while (Spline->GetDistanceAlongSplineAtSplinePoint(removePointIndex) + 10 < playerDistance && SplineMesh.Num() > 0 && SplineMesh[0])
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("cleaning up %d %d %f %f"), SplineMesh.Num(), removePointIndex, Spline->GetDistanceAlongSplineAtSplinePoint(removePointIndex), playerDistance);

	//	//currentPoint--;
	//}

	//while (Spline->GetDistanceAlongSplineAtSplinePoint(removePointIndex) + 10 < playerDistance )
	//{
	if (remove) {
		Spline->RemoveSplinePoint(0, false);
		//SplineMesh[0]->UnregisterComponent();
		//SplineMesh[0]->DetachFromParent();
		//SplineMesh[0]->DestroyComponent();
		//SplineMesh.RemoveAt(0);
		if (insertedPoints > (initializationSize * 2) && insertedPoints % 50 == 0 && ObstaclesActor.Num() > 0) {
			if (ObstaclesActor[0])
			{
				ObstaclesActor[0]->Destroy();
				ObstaclesActor.RemoveAt(0);
			}
		}
		currentPoint--;
	}
	//}

	bool placedObstacle = false;
	for (; currentPoint < Spline->GetNumberOfSplinePoints() - 1; currentPoint++)
	{
		Spline->GetLocationAndTangentAtSplinePoint(currentPoint, locStart, tanStart, ESplineCoordinateSpace::Local);
		Spline->GetLocationAndTangentAtSplinePoint(currentPoint + 1, locEnd, tanEnd, ESplineCoordinateSpace::Local);

		if (currentSplineMesh >= SplineMesh.Num())
			currentSplineMesh = 0;
		//UE_LOG(LogTemp, Display, TEXT("%d"), currentSplineMesh);
		auto splineMesh = SplineMesh[currentSplineMesh++];
		splineMesh->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd);
		splineMesh->SetStartOffset(FVector2D(-15, 0));
		splineMesh->SetEndOffset(FVector2D(-15, 0));
		if (insertedPoints % 50 == 0 && !placedObstacle)
		{
			UWorld* const World = GetWorld();
			if (World && Obstacles.Num() > 0)
			{
				auto transform = Spline->GetTransformAtSplinePoint(currentPoint, ESplineCoordinateSpace::World);
				auto rotation = FRotator(transform.GetRotation()).Add(90, 0, 0);
				int obstacle = FMath::Rand() % Obstacles.Num();
				auto actor = World->SpawnActor<AObstacle>(Obstacles[obstacle], transform.GetLocation(), rotation);
				//UE_LOG(LogTemp, Display, TEXT("%s %d %s"), *transform.GetLocation().ToCompactString(), i, *actor->GetName());
				if (actor) {
					int random = FMath::Rand() % 360;
					auto newRotation = UKismetMathLibrary::ComposeRotators(actor->GetActorRotation(), UKismetMathLibrary::RotatorFromAxisAndAngle(actor->GetActorUpVector(), random));

					actor->SetActorRotation(newRotation);
					AObstacle* createObstacle = Cast<AObstacle>(actor);
					if (createObstacle) {
						int random = FMath::Rand() % 100;
						float velocity = 0;
						if (random < 50) {
							velocity = 0;
						}
						else if (random < 80) {
							velocity = 0.5;
						}
						else {
							velocity = 1;
						}
						createObstacle->rotationVelocity = velocity;
						ObstaclesActor.Add(actor);
					}
				}
			}
			placedObstacle = true;
		}
	}
}
