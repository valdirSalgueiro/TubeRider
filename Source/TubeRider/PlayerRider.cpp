// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRider.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" 


// Sets default values
APlayerRider::APlayerRider()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));;
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// debug visual
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 400.f;
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

