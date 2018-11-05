// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRider.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"


// Sets default values
APlayerRider::APlayerRider()
{
	settings = GameplaySettings::GetSettings();

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));;
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(1.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	SphereComponent->SetHiddenInGame(true);
	SphereComponent->SetSimulatePhysics(false);
	SphereComponent->SetEnableGravity(false);


	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 0.f;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->SetupAttachment(RootComponent);

	FlashLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	FlashLightComponent->SetupAttachment(Camera);
	FlashLightComponent->RelativeLocation = FVector(10, 0, 4.f);
	FlashLightComponent->AttenuationRadius = 1000.f;
	FlashLightComponent->bAffectsWorld = true;
	FlashLightComponent->SetMobility(EComponentMobility::Movable);

	angle = 90;
	distance = 0;
	playerVelocity = settings->playerInitialVelocity;
}

// Called when the game starts or when spawned
void APlayerRider::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<ATube> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		tube = *ActorItr;
		break;
	}
	distance = 0;
}
// Called every frame
void APlayerRider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	settings->update(DeltaTime);

	playerVelocity = settings->lerpPlayerVelocity();

	if (tube != NULL && tube->IsReady())
	{
		auto SplineComponent = tube->GetSpline();
		if (SplineComponent != NULL) {
			auto transform = SplineComponent->GetTransformAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
			FVector location = transform.GetLocation();
			if (!MovementInput.IsZero())
			{
				MovementInput = MovementInput.GetSafeNormal();
				angle += MovementInput.Y * 300 * DeltaTime;
				angle = (int)angle % 360;
			}
			auto vecRight = FRotationMatrix(transform.Rotator()).GetScaledAxis(EAxis::Y);
			auto vecUp = FRotationMatrix(transform.Rotator()).GetScaledAxis(EAxis::Z);
			transform.SetLocation(location + (-vecRight * FMath::Cos(FMath::DegreesToRadians(angle)) * 30) + (vecUp * FMath::Sin(FMath::DegreesToRadians(angle)) * 30));
			SetActorTransform(transform);
			
			distance += DeltaTime * playerVelocity;

			float firstPointDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(1);
			//UE_LOG(LogTemp, Display, TEXT("%f %f"), distance, firstPointDistance);
			if (distance > firstPointDistance) {
				tube->InsertNewPoints(distance);
				tube->CreateSplineMesh(true);
				//UE_LOG(LogTemp, Display, TEXT("new distance%f"), distance - firstPointDistance);
				distance -= firstPointDistance;
			}

		}
	}
}

// Called to bind functionality to input
void APlayerRider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerRider::MoveRight);
}

//Input functions
void APlayerRider::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}
//Input functions
void APlayerRider::Shake(float scale)
{
	if (tube != NULL && tube->IsReady() && cameraShake != NULL)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(cameraShake, scale);
	}
}

