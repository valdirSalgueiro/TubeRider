// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRider.h"



// Sets default values
APlayerRider::APlayerRider()
{
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
	//SetActorRelativeLocation(FVector(60, 0, 0));

	// debug visual
	//UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	//SphereVisual->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	//if (SphereVisualAsset.Succeeded())
	//{
	//	SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
	//	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -15.0f));
	//	SphereVisual->SetWorldScale3D(FVector(0.3f));
	//	SphereVisual->SetGenerateOverlapEvents(false);
	//}

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 0.f;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	//Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->SetupAttachment(RootComponent);
	//Camera->SetRelativeLocation(FVector(60, 0, 0));
	//Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	auto FlashLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	FlashLightComponent->SetupAttachment(RootComponent);
	FlashLightComponent->RelativeLocation = FVector(10, 0, 4.f);
	FlashLightComponent->AttenuationRadius = 1000.f;
	FlashLightComponent->bAffectsWorld = true;
	FlashLightComponent->SetMobility(EComponentMobility::Movable);

	angle = 90;
	distance = 0;
	playerVelocity = 300;
	
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
	auto SplineComponent = tube->GetSpline();
	distance = SplineComponent->GetSplineLength() * 0.5f;
}
float lastX = 0;
float lastY = 0;
float lastZ = 0;

// Called every frame
void APlayerRider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tube != NULL && tube->IsReady())
	{
		auto SplineComponent = tube->GetSpline();
		if (SplineComponent != NULL) {
			int numberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();
			if (numberOfSplinePoints > 100) {
				float totalLength = SplineComponent->GetSplineLength();

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

				while (distance >= totalLength / 2.0f)
				{
					for (int i = 0; i < 4; i++) {
						tube->InsertNewPoints(distance);
						totalLength = SplineComponent->GetSplineLength();
					}
				}
				tube->CreateSplineMesh();
				distance += DeltaTime * playerVelocity;
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

