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
			auto transform = Spline->GetTransformAtSplinePoint(currentPoint, ESplineCoordinateSpace::World);
			auto rotation = FRotator(transform.GetRotation()).Add(90, 0, 0);
			SpawnObstacles(World, Obstacles, playerAngle, transform, rotation);
		}
		lastPlacedSecond = elapsedSeconds;
	}
}

void ObstacleSpawner::SpawnObstacles(UWorld * World, TArray<TSubclassOf<AObstacle>> &Obstacles, float playerAngle, FTransform &transform, FRotator &rotation)
{
	int difficulty = gameplaySettings->lerpDifficulty() * 100;
	if (difficulty > 80) {
	}
	else if (difficulty > 60)
	{
	}
	else if (difficulty > 40)
	{
	}
	else if (difficulty > 20)
	{
	}
	else
	{
		int random = 0;
		float velocity;
		AObstacle* newObstacle = World->SpawnActor<AObstacle>(Obstacles[0], transform.GetLocation(), rotation);
		if (newObstacle) {
			auto newRotation = UKismetMathLibrary::ComposeRotators(newObstacle->GetActorRotation(), UKismetMathLibrary::RotatorFromAxisAndAngle(newObstacle->GetActorUpVector(), random));

			newObstacle->SetActorRotation(newRotation);
			if (newObstacle) {
				int random = FMath::Rand() % 100;
				if (random < 50) {
					velocity = 0;
				}
				else if (random < 80) {
					velocity = 0.5;
				}
				else {
					velocity = 1;
				}
				newObstacle->rotationVelocity = velocity;
			}
		}
	}
}
