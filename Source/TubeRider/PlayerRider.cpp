// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRider.h"


// Sets default values
APlayerRider::APlayerRider()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));;
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	//CameraSpringArm->SetRelativeLocationAndRotation(FVector(60.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	CameraSpringArm->TargetArmLength = 400.f;
	//CameraSpringArm->bEnableCameraLag = true;
	//CameraSpringArm->CameraLagSpeed = 3.0f;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	distance = 0;
	angle = 90;
}

// Called when the game starts or when spawned
void APlayerRider::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerRider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	distance += DeltaTime * 300.0f;

	if (tube != NULL)
	{
		auto SplineComponent = tube->GetSpline();
		if (SplineComponent != NULL) {
			int numberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();
			float totalLength = SplineComponent->GetSplineLength();
			if (distance >= totalLength / 2.5f)
			{
				tube->InsertNewPoints();
				numberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();
				totalLength = SplineComponent->GetSplineLength();
			}

			auto transform = SplineComponent->GetTransformAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
			FVector location = transform.GetLocation();
			if (!MovementInput.IsZero())
			{
				MovementInput = MovementInput.GetSafeNormal();
				angle += MovementInput.Y * 5.0f;
				angle = (int)angle % 360;
			}
			auto vecRight = FRotationMatrix(transform.Rotator()).GetScaledAxis(EAxis::Y);
			auto vecUp = FRotationMatrix(transform.Rotator()).GetScaledAxis(EAxis::Z);
			transform.SetLocation(location + (vecRight * -FMath::Cos(FMath::DegreesToRadians(angle)) * 30) + (vecUp * FMath::Sin(FMath::DegreesToRadians(angle)) * 30));
			SetActorTransform(transform);
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

