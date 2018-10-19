// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tube.generated.h"

UCLASS()
class TUBERIDER_API ATube : public AActor
{
	GENERATED_BODY()
	
public:

	bool created;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, Category = Spline)
		USplineComponent* Spline;
	UPROPERTY()
		TArray<USplineMeshComponent *> SplineMesh;

	ATube();

	USplineComponent* GetSpline() {
		return Spline;
	}

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform &Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InsertNewPoints();

private:
	void createSplineMesh();

	int currentPoints;

	
	
};
