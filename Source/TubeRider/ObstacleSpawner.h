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

	void SpawnObstacles(UWorld * World, TArray<TSubclassOf<AObstacle>> &Obstacles, float playerAngle, USplineComponent* Spline, int currentPoint);

	void SpawnDifficulty5(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, FTransform &initialTransform, FRotator &rotation, float &initialDistance, USplineComponent * Spline, float velocity);

	void SpawnDifficulty4(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, FTransform &initialTransform, FRotator &rotation, float initialDistance, USplineComponent * Spline, float velocity);

	void SpawnDifficulty3(TArray<TSubclassOf<AObstacle>> & Obstacles, UWorld * World, FTransform &initialTransform, FRotator &rotation);

	void SpawnDifficulty2(TArray<TSubclassOf<AObstacle>> & Obstacles, UWorld * World, FTransform &initialTransform, FRotator &rotation);

	void SpawnDifficulty1(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, FTransform &initialTransform, FRotator &rotation, int angle, float velocity);

	void SpawnDifficulty1_(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, int obstacle, FTransform & initialTransform, FRotator & rotation, float velocity, int angle);

	AObstacle* SpawnSingleObstacle(UWorld * World, TArray<TSubclassOf<AObstacle>> & Obstacles, int obstacle, FTransform & transform, FRotator & rotation, float velocity);

private:
	GameplaySettings* gameplaySettings;

	float difficulty;

	float lastPlacedSecond;
};
