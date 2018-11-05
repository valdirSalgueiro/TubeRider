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

	AObstacle*  SpawnObject(UWorld* world, USplineComponent* Spline, TArray<TSubclassOf<AObstacle>> obstacles, int currentPoint, float elapsedSeconds);

private:
	GameplaySettings* gameplaySettings;

	float difficulty;

	float lastPlacedSecond;
};
