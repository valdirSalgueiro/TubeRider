// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Obstacle.h"
#include "GameplaySettings.h"
#include "CoreMinimal.h"

/**
 * 
 */
class TUBERIDER_API ObstacleSpawner
{
public:

	ObstacleSpawner();
	~ObstacleSpawner();

	void SpawnObjects(UWorld* world, USplineComponent* Spline, TArray<TSubclassOf<AObstacle>> obstacles, float playerAngle, int currentPoint, float elapsedSeconds);

	void SpawnObstacles(UWorld * World, TArray<TSubclassOf<AObstacle>> &Obstacles, float playerAngle, FTransform &transform, FRotator &rotation);

private:
	GameplaySettings* gameplaySettings;

	float difficulty;

	float lastPlacedSecond;
};
