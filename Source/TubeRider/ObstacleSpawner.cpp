// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstacleSpawner.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplaySettings.h"

ObstacleSpawner::ObstacleSpawner()
{
	lastPlacedSecond = 0;
	gameplaySettings = GameplaySettings::GetSettings();
	minVelocity = 0.2f;
	midVelocity = 0.3f;
	maxVelocity = 0.5f;
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
	UE_LOG(LogTemp, Display, TEXT("diff: %d"), difficulty);


	if (difficulty > 80) {
		int random = FMath::Rand() % 10;
		if (random == 0)
		{
			int randomDirection = FMath::Rand() % 3;
			float velocity = 0;
			switch (randomDirection) {
			case 0:
				velocity = 0;
				break;
			case 1:
				velocity = midVelocity;
				break;
			case 2:
				velocity = -midVelocity;
				break;
			}
			SpawnDifficulty5(World, Obstacles, initialTransform, rotation, initialDistance, Spline, velocity);
		}
		else if (random == 1)
		{
			int randomDirection = FMath::Rand() % 2;
			float velocity = 0;
			switch (randomDirection) {
			case 0:
				velocity = 0;
				break;
			case 1:
				velocity = maxVelocity;
				break;
			case 2:
				velocity = -maxVelocity;
				break;
			}
			SpawnDifficulty4(World, Obstacles, initialTransform, rotation, initialDistance, Spline, velocity);
		}
		else if (random == 2)
		{
			int angle = 0;
			int randomDirection = FMath::Rand() % 2;
			float velocity = 0;
			switch (randomDirection) {
			case 0:
				velocity = 0;
				break;
			case 1:
				velocity = midVelocity;
				break;
			case 2:
				velocity = -midVelocity;
				break;
			}
			for (int i = 0; i < 3; i++)
			{
				SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
				SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, velocity);
				initialDistance -= 30;
				initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
			}
		}
		else if (random == 3)
		{
			int angle = 0;
			for (int i = 0; i < 5; i++)
			{
				SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, -midVelocity);
				SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, -midVelocity);
				initialDistance -= 30;
				initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
			}
		}
		else
		{
			int angle = FMath::Rand() % 360;
			for (int i = 0; i < 10; i++)
			{
				SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, 0);
				SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, 0);
				initialDistance -= 30;
				initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
			}
		}
	}
	else if (difficulty > 70)
	{
		int angle = 0;
		int randomDirection = FMath::Rand() % 2;
		float velocity = 0;
		switch (randomDirection) {
		case 0:
			velocity = 0;
			break;
		case 1:
			velocity = midVelocity;
			break;
		case 2:
			velocity = -midVelocity;
			break;
		}
		for (int i = 0; i < 3; i++)
		{
			SpawnDifficulty1_(World, Obstacles, 1, initialTransform, rotation, angle, velocity);
			initialDistance -= 30;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
	else if (difficulty > 50)
	{
		int randomDirection = FMath::Rand() % 2;
		float velocity = 0;
		switch (randomDirection) {
		case 0:
			velocity = 0;
			break;
		case 1:
			velocity = minVelocity;
			break;
		case 2:
			velocity = -minVelocity;
			break;
		}
		SpawnDifficulty5(World, Obstacles, initialTransform, rotation, initialDistance, Spline, velocity);
	}
	else if (difficulty > 40)
	{
		int random = FMath::Rand() % 3;
		if (random == 0)
		{
			SpawnDifficulty4(World, Obstacles, initialTransform, rotation, initialDistance, Spline, minVelocity);
		}
		else if (random == 1)
		{
			SpawnDifficulty4(World, Obstacles, initialTransform, rotation, initialDistance, Spline, -midVelocity);
		}
		else
		{
			SpawnDifficulty3(Obstacles, World, initialTransform, rotation);
		}
	}
	else if (difficulty > 30)
	{
		int randomDirection = FMath::Rand() % 2;
		float velocity = 0;
		switch (randomDirection) {
		case 0:
			velocity = 0;
			break;
		case 1:
			velocity = minVelocity;
			break;
		case 2:
			velocity = -minVelocity;
			break;
		}
		SpawnDifficulty4(World, Obstacles, initialTransform, rotation, initialDistance, Spline, velocity);
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
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, velocity);
	}
	else if (random == 1) {
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, -velocity);
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, -velocity);
	}
	else if (random == 2) {
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 180, velocity);
	}
	else if (random == 3) {
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, -velocity);
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle + 270, -velocity);
	}
}

void ObstacleSpawner::SpawnDifficulty4(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, FTransform &initialTransform, FRotator &rotation, float initialDistance, USplineComponent * Spline, float velocity)
{
	int angle = FMath::Rand() % 360;
	int random = FMath::Rand() % 3;
	int randomObstacle = FMath::Rand() % 2;
	if (random == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			SpawnDifficulty1_(World, Obstacles, randomObstacle, initialTransform, rotation, angle, velocity);
			initialDistance -= 10;
			angle += 10;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
	else if (random == 1) {
		for (int i = 0; i < 2; i++)
		{
			SpawnDifficulty1_(World, Obstacles, randomObstacle, initialTransform, rotation, angle, -velocity);
			initialDistance -= 10;
			angle -= 10;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
	else if (random == 2) {
		for (int i = 0; i < 2; i++)
		{
			SpawnDifficulty1_(World, Obstacles, randomObstacle, initialTransform, rotation, angle, velocity);
			initialDistance -= 10;
			angle += 30;
			initialTransform = Spline->GetTransformAtDistanceAlongSpline(initialDistance, ESplineCoordinateSpace::World);
		}
	}
}

void ObstacleSpawner::SpawnDifficulty3(TArray<TSubclassOf<AObstacle>> & Obstacles, UWorld * World, FTransform &initialTransform, FRotator &rotation)
{
	float velocity = maxVelocity;
	int randomDirection = FMath::Rand() % 2;
	velocity = randomDirection ? velocity : -velocity;
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
		velocity = minVelocity;
	}
	else {
		velocity = midVelocity;
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
		SpawnDifficulty1_(World, Obstacles, 0, initialTransform, rotation, angle, velocity);
	else
		SpawnDifficulty1_(World, Obstacles, 1, initialTransform, rotation, angle, velocity);
}

void ObstacleSpawner::SpawnDifficulty1_(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, int obstacle, FTransform & initialTransform, FRotator & rotation, int angle, float velocity)
{
	auto newObstacle = SpawnSingleObstacle(World, Obstacles, obstacle, initialTransform, rotation, velocity);
	auto newRotation = UKismetMathLibrary::ComposeRotators(newObstacle->GetActorRotation(), UKismetMathLibrary::RotatorFromAxisAndAngle(newObstacle->GetActorUpVector(), angle));
	newObstacle->SetActorRotation(newRotation);
}

AObstacle* ObstacleSpawner::SpawnSingleObstacle(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, int obstacle, FTransform & transform, FRotator & rotation, float velocity)
{
	AObstacle* newObstacle = World->SpawnActor<AObstacle>(Obstacles[obstacle], transform.GetLocation(), rotation);
	if (newObstacle) {
		newObstacle->SetLifeSpan(gameplaySettings->lerpTimeSpan());
		if (newObstacle)
		{
			newObstacle->rotationVelocity = velocity;
		}
	}
	return newObstacle;
}
