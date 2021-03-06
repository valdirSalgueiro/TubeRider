// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "ObstacleSpawner.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "TubeRiderGameInstance.h"
#include "MenuSystem/MainMenu.h"
#include "Tube.generated.h"

class APlayerRider;

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

	ATube();

	USplineComponent* GetSpline() {
		return Spline;
	}

protected:
	virtual void BeginPlay() override;

	void InitializeGame();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetNewAngle();

	void InsertNewPoints();

	bool IsReady() {
		return isReady;
	}

	void CreateSplineMesh(bool remove);

	void SetPlayer(APlayerRider* _player) {
		player = _player;
	}

private:

	int GetNewRandomAngle();
	void GetHorizontalAngle();
	void GetVerticalAngle();
	void NextMainMenuScreen();

	int currentPoint;

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

	UTubeRiderGameInstance* gameInstance;
	APlayerRider* player;

	UMainMenu* Menu;
	FTimerHandle menuTimer;

	float screenTimer;

	int currentScreen;
};
