// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstacleSpawner.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplaySettings.h"

ObstacleSpawner::ObstacleSpawner()
{
	lastPlacedSecond = 0;
	gameplaySettings = GameplaySettings::GetSettings();
}

ObstacleSpawner::~ObstacleSpawner()
{
}

void ObstacleSpawner::SpawnObjects(UWorld* World, USplineComponent* Spline, TArray<TSubclassOf<AObstacle>> Obstacles, float playerAngle, int currentPoint, float elapsedSeconds) {
	float obstacleInterval = gameplaySettings->lerpObstacleInterval();
	if (elapsedSeconds > lastPlacedSecond + obstacleInterval)
	{
		if (World && Obstacles.Num() > 0)
		{
			SpawnObstacles(World, Obstacles, playerAngle, Spline, currentPoint);
		}
		lastPlacedSecond = elapsedSeconds;
	}
}

void ObstacleSpawner::SpawnObstacles(UWorld * World, TArray<TSubclassOf<AObstacle>> &Obstacles, float playerAngle, USplineComponent* Spline, int currentPoint)
{
	float initialDistance = Spline->GetDistanceAlongSplineAtSplinePoint(currentPoint);
	auto initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
	auto rotation = FRotator(initialTransform.GetRotation()).Add(90, 0, 0);

	int difficulty = gameplaySettings->lerpDifficulty() * 100;

	if (difficulty > 80) {
		int random = FMath::Rand() % 2;
		if (random == 0)
		{
			SpawnDifficulty5(World, Obstacles, initialTransform, rotation, initialDistance, Spline, 1.f);
		}
		else if (random == 1)
		{
			SpawnDifficulty4(World, Obstacles, initialTransform, rotation, initialDistance, Spline, 1.f);
		}
	}
	if (difficulty > 50)
	{
		SpawnDifficulty5(World, Obstacles, initialTransform, rotation, initialDistance, Spline, 0.5f);
	}
	else if (difficulty > 40)
	{
		int random = FMath::Rand() % 3;
		if (random == 0)
		{
			SpawnDifficulty4(World, Obstacles, initialTransform, rotation, initialDistance, Spline, 0.5f);
		}
		else if (random == 1)
		{
			SpawnDifficulty4(World, Obstacles, initialTransform, rotation, initialDistance, Spline, 1.f);
		}
		else
		{
			SpawnDifficulty3(Obstacles, World, initialTransform, rotation);
		}
	}
	else if (difficulty > 30)
	{
		SpawnDifficulty4(World, Obstacles, initialTransform, rotation, initialDistance, Spline, 0.5f);
	}
	else if (difficulty > 20)
	{
		SpawnDifficulty3(Obstacles, World, initialTransform, rotation);
	}
	else if (difficulty > 10)
	{
		SpawnDifficulty2(Obstacles, World, initialTransform, rotation);
	}
	else
	{
		SpawnDifficulty1(World, Obstacles, initialTransform, rotation, playerAngle, 0);
	}
}

void ObstacleSpawner::SpawnDifficulty5(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, FTransform &initialTransform, FRotator &rotation, float &initialDistance, USplineComponent * Spline, float velocity)
{
	int angle = FMath::Rand() % 360;
	int random = FMath::Rand() % 4;
	if (random == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle+180, velocity);
			initialDistance += 50;
			angle += 10;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
	else if (random == 1) {
		for (int i = 0; i < 2; i++)
		{
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, velocity);
			initialDistance += 50;
			angle += 10;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
		for (int i = 0; i < 3; i++)
		{
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, velocity);
			initialDistance += 50;
			angle -= 10;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
	else if (random == 2) {
		for (int i = 0; i < 2; i++)
		{
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, velocity);
			initialDistance += 50;
			angle += 30;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
		for (int i = 0; i < 3; i++)
		{
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, velocity);
			initialDistance -= 50;
			angle += 30;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
	else if (random == 3) {
		for (int i = 0; i < 2; i++)
		{
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 270, velocity);
			initialDistance += 50;
			angle += 30;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
		for (int i = 0; i < 3; i++)
		{
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
			SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 270, velocity);
			initialDistance -= 50;
			angle += 30;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
}

void ObstacleSpawner::SpawnDifficulty4(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, FTransform &initialTransform, FRotator &rotation, float initialDistance, USplineComponent * Spline, float velocity)
{
	int angle = FMath::Rand() % 360;
	int random = FMath::Rand() % 3;
	if (random == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			SpawnDifficulty1(World, Obstacles, initialTransform, rotation, angle, velocity);
			initialDistance += 50;
			angle += 10;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
	else if (random == 1) {
		for (int i = 0; i < 2; i++)
		{
			SpawnDifficulty1(World, Obstacles, initialTransform, rotation, angle, velocity);
			initialDistance += 50;
			angle += 10;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
		for (int i = 0; i < 3; i++)
		{
			SpawnDifficulty1(World, Obstacles, initialTransform, rotation, angle, velocity);
			initialDistance += 50;
			angle -= 10;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
	else if (random == 2) {
		for (int i = 0; i < 2; i++)
		{
			SpawnDifficulty1(World, Obstacles, initialTransform, rotation, angle, velocity);
			initialDistance += 50;
			angle += 30;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
		for (int i = 0; i < 3; i++)
		{
			SpawnDifficulty1(World, Obstacles, initialTransform, rotation, angle, velocity);
			initialDistance -= 50;
			angle += 30;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
}

void ObstacleSpawner::SpawnDifficulty3(TArray<TSubclassOf<AObstacle>> & Obstacles, UWorld * World, FTransform &initialTransform, FRotator &rotation)
{
	float velocity = 1;
	int obstacle = FMath::Rand() % Obstacles.Num();
	int angle = FMath::Rand() % 360;
	auto newObstacle = SpawnSingleObstacle(World, Obstacles, obstacle, initialTransform, rotation, velocity);
	auto newRotation = UKismetMathLibrary::ComposeRotators(newObstacle->GetActorRotation(), UKismetMathLibrary::RotatorFromAxisAndAngle(newObstacle->GetActorUpVector(), angle));
	newObstacle->SetActorRotation(newRotation);
}

void ObstacleSpawner::SpawnDifficulty2(TArray<TSubclassOf<AObstacle>> & Obstacles, UWorld * World, FTransform &initialTransform, FRotator &rotation)
{
	int angle = FMath::Rand() % 360;
	int random = FMath::Rand() % 100;
	float velocity = 0;
	if (random < 80) {
		velocity = 0.5;
	}
	else {
		velocity = 1;
	}

	int obstacle = FMath::Rand() % Obstacles.Num();
	auto newObstacle = SpawnSingleObstacle(World, Obstacles, obstacle, initialTransform, rotation, velocity);
	auto newRotation = UKismetMathLibrary::ComposeRotators(newObstacle->GetActorRotation(), UKismetMathLibrary::RotatorFromAxisAndAngle(newObstacle->GetActorUpVector(), angle));
	newObstacle->SetActorRotation(newRotation);
}

void ObstacleSpawner::SpawnDifficulty1(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, FTransform &initialTransform, FRotator &rotation, int angle, float velocity)
{
	int obstacle = FMath::Rand() % 2;
	if (!obstacle)
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, velocity, angle);
	else 
		SpawnDifficulty1_(World, Obstacles, 1, initialTransform, rotation, velocity, angle);
}

void ObstacleSpawner::SpawnDifficulty1_(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, int obstacle, FTransform & initialTransform, FRotator & rotation, float velocity, int angle)
{
	auto newObstacle = SpawnSingleObstacle(World, Obstacles, obstacle, initialTransform, rotation, velocity);
	auto newRotation = UKismetMathLibrary::ComposeRotators(newObstacle->GetActorRotation(), UKismetMathLibrary::RotatorFromAxisAndAngle(newObstacle->GetActorUpVector(), angle));
	newObstacle->SetActorRotation(newRotation);
}

AObstacle* ObstacleSpawner::SpawnSingleObstacle(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, int obstacle, FTransform & transform, FRotator & rotation, float velocity)
{
	AObstacle* newObstacle = World->SpawnActor<AObstacle>(Obstacles[obstacle], transform.GetLocation(), rotation);
	if (newObstacle) {
		newObstacle->SetLifeSpan(10);
		if (newObstacle) {

			newObstacle->rotationVelocity = velocity;
		}
	}
}
