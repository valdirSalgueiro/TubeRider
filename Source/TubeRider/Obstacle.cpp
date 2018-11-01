// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstacle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/BodySetup.h"


// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create the static mesh component
	//simplecol

	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMesh"));
	RootComponent = ObstacleMesh;

	//auto boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::HandleCollision);//for collision handling
	//boxCollision->InitBoxExtent(FVector(50, 100, 30)*0.6f);
	//boxCollision->SetupAttachment(RootComponent);
	//boxCollision->SetCollisionProfileName(FName("OverlapAll"));
	//boxCollision->SetGenerateOverlapEvents(true);
	//boxCollision->SetMobility(EComponentMobility::Movable);
	//boxCollision->SetEnableGravity(false);
	//boxCollision->SetSimulatePhysics(false);
	SetActorEnableCollision(false);
	/*ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMesh"));
	RootComponent = ObstacleMesh;*/

	
	

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	ObstacleMesh->GetBodySetup()->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseSimpleAsComplex;
	ObstacleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ObstacleMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	ObstacleMesh->SetMobility(EComponentMobility::Static);
	ObstacleMesh->SetGenerateOverlapEvents(true);
	ObstacleMesh->SetSimulatePhysics(false);
	ObstacleMesh->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::HandleCollision);
	//auto boxCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	//boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::HandleCollision);//for collision handling
	//boxCollision->InitBoxExtent(FVector(50, 100, 30)*0.6f);
	//boxCollision->SetupAttachment(RootComponent);
	//boxCollision->SetCollisionProfileName(FName("OverlapAll"));
	//boxCollision->SetGenerateOverlapEvents(true);
	//boxCollision->SetMobility(EComponentMobility::Movable);
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AObstacle::HandleCollision(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& OverlapInfo)
{
	FString name = Other->GetName();
	UE_LOG(LogTemp, Display, TEXT("colidiu: %s"), *name);
}


