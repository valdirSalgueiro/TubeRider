// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstacle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerRider.h"


// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the static mesh component
	//simplecol

	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMesh"));
	RootComponent = ObstacleMesh;
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	ObstacleMesh->GetBodySetup()->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseSimpleAsComplex;
	ObstacleMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ObstacleMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	ObstacleMesh->SetMobility(EComponentMobility::Movable);
	ObstacleMesh->SetGenerateOverlapEvents(true);
	ObstacleMesh->SetSimulatePhysics(false);
	ObstacleMesh->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::HandleCollision);	
	rotationVelocity = 0;
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	angle = DeltaTime * rotationVelocity * 500;
	//UE_LOG(LogTemp, Display, TEXT("velocity: %f"), rotationVelocity);
	auto newRotation = UKismetMathLibrary::ComposeRotators(GetActorRotation(), UKismetMathLibrary::RotatorFromAxisAndAngle(GetActorUpVector(), angle));
	SetActorRotation(newRotation);
}

void AObstacle::HandleCollision(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& OverlapInfo)
{
	FString name = Other->GetName();
	UE_LOG(LogTemp, Display, TEXT("colidiu: %s"), *name);
	APlayerRider* player = Cast<APlayerRider>(Other);
	if (player != NULL) {
		player->Shake(1.f);
	}
}


