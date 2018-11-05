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

AObstacle* ObstacleSpawner::SpawnObject(UWorld* World, USplineComponent* Spline, TArray<TSubclassOf<AObstacle>> Obstacles, int currentPoint, float elapsedSeconds) {
	AObstacle* newObstacle = NULL;
	float obstacleInterval = gameplaySettings->lerpObstacleInterval();
	UE_LOG(LogTemp, Display, TEXT("%f"), obstacleInterval);
	if (elapsedSeconds > lastPlacedSecond + obstacleInterval)
	{
		if (World && Obstacles.Num() > 0)
		{
			auto transform = Spline->GetTransformAtSplinePoint(currentPoint, ESplineCoordinateSpace::World);
			auto rotation = FRotator(transform.GetRotation()).Add(90, 0, 0);
			int obstacle = FMath::Rand() % Obstacles.Num();
			newObstacle = World->SpawnActor<AObstacle>(Obstacles[obstacle], transform.GetLocation(), rotation);
			if (newObstacle) {
				int random = FMath::Rand() % 360;
				auto newRotation = UKismetMathLibrary::ComposeRotators(newObstacle->GetActorRotation(), UKismetMathLibrary::RotatorFromAxisAndAngle(newObstacle->GetActorUpVector(), random));

				newObstacle->SetActorRotation(newRotation);
				if (newObstacle) {
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
					newObstacle->rotationVelocity = velocity;
				}
			}
		}
		lastPlacedSecond = elapsedSeconds;
	}
	return newObstacle;
}