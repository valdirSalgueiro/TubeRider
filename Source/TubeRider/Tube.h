// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "ObstacleSpawner.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "Tube.generated.h"

UCLASS()
class TUBERIDER_API ATube : public AActor
{
	GENERATED_BODY()

public:

	struct ObstacleDistance {
		AObstacle* obstacle;
		float distance;
	};

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* Mesh;

	USplineComponent* Spline;

	TArray<USplineMeshComponent *> SplineMesh;

	UPROPERTY(EditAnywhere, Category = Obstacle)
		TArray<TSubclassOf<AObstacle>> Obstacles;

	UPROPERTY(EditAnywhere, Category = Obstacle)
		UMaterial* Material;

	ATube();

	USplineComponent* GetSpline() {
		return Spline;
	}

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetNewAngle();

	void InsertNewPoints(float distance);

	bool IsReady() {
		return isReady;
	}

	void CreateSplineMesh(bool remove);

private:

	int GetNewRandomAngle();
	void GetHorizontalAngle();
	void GetVerticalAngle();

	int currentPoint;

	int yDirection;
	int zDirection;

	TArray<FVector> SplinePoints;
	FVector lastPoint;
	float angleH;
	float angleV;
	int angleHDest;
	int angleVDest;

	float xDir;
	float yDir;	

	int removePointIndex;
	bool isReady;
	float elapsedSeconds;
	int initializationSize;
	int currentSplineMesh;

	ObstacleSpawner* spawner;
	UWorld* world;

	TArray<ObstacleDistance> ObstaclesActor;
};
