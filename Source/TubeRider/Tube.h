// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "Tube.generated.h"

UCLASS()
class TUBERIDER_API ATube : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* Mesh;

	USplineComponent* Spline;

	TArray<USplineMeshComponent *> SplineMesh;

	UPROPERTY(EditAnywhere, Category = Obstacle)
		TArray<TSubclassOf<AObstacle>> Obstacles;

	UPROPERTY(EditAnywhere, Category = Obstacle)
		UMaterial* Material;

	TArray<AActor*> ObstaclesActor;

	ATube();

	USplineComponent* GetSpline() {
		return Spline;
	}

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InsertNewPoints(float distance);

	bool IsReady() {
		return isReady;
	}

	void CreateSplineMesh(bool remove);

private:

	int GetNewRandomAngle();

	int currentPoint;

	int yDirection;
	int zDirection;

	TArray<FVector> SplinePoints;
	FVector lastPoint;
	float angleH;
	float angleV;
	float angleHDest;
	float angleVDest;

	float xDir;
	float yDir;	

	float playerDistance;

	int removePointIndex;

	bool isReady;

	int insertedPoints;

	int initializationSize;

	int currentSplineMesh;
};
