// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRider.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APlayerRider::APlayerRider()
{
	settings = GameplaySettings::GetSettings();

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	auto scene = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));;
	RootComponent = scene;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->TargetArmLength = 0;
	CameraSpringArm->SocketOffset = FVector(0, 0, 20);
	CameraSpringArm->AttachTo(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->AttachTo(CameraSpringArm);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));;
	SphereComponent->InitSphereRadius(1.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	SphereComponent->SetHiddenInGame(true);
	SphereComponent->SetSimulatePhysics(false);
	SphereComponent->SetEnableGravity(false);
	SphereComponent->SetupAttachment(CameraSpringArm);

	//FlashLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	//FlashLightComponent->SetupAttachment(Camera);
	//FlashLightComponent->RelativeLocation = FVector(10, 0, 4.f);
	//FlashLightComponent->AttenuationRadius = 1000.f;
	//FlashLightComponent->bAffectsWorld = true;
	//FlashLightComponent->SetMobility(EComponentMobility::Movable);

	angle = 0;
	distance = 0;
	playerVelocity = settings->playerInitialVelocity;
	hits = 0;
}

// Called when the game starts or when spawned
void APlayerRider::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<ATube> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		tube = *ActorItr;
		tube->SetPlayer(this);
		break;
	}
	Reset();
}
void APlayerRider::Reset()
{
	settings->start();
	distance = 0;
	Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
	Camera->PostProcessSettings.VignetteIntensity = 0;
}
// Called every frame
void APlayerRider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (tube != NULL && tube->IsReady())
	{
		settings->update(DeltaTime);
		playerVelocity = settings->lerpPlayerVelocity();

		auto SplineComponent = tube->GetSpline();
		if (SplineComponent != NULL) {
			auto transform = SplineComponent->GetTransformAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
			FVector location = transform.GetLocation();
			if (!MovementInput.IsZero())
			{
				MovementInput = MovementInput.GetSafeNormal();
				angle += MovementInput.Y * 200 * DeltaTime;
				angle = (int)angle % 360;
			}

			//transform.SetLocation(location);
			CameraSpringArm->SetRelativeRotation(FRotator(0, 0, angle));			
			SetActorTransform(transform);
			
			distance += DeltaTime * playerVelocity;

			float firstPointDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(1);
			if (distance > firstPointDistance) {
				tube->InsertNewPoints();
				tube->CreateSplineMesh(true);
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
		hits++;
		float vignette = hits;
		vignette = FMath::Min(3.f, vignette);
		Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
		Camera->PostProcessSettings.VignetteIntensity = vignette;
	}
}

