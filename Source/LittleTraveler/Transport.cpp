// Fill out your copyright notice in the Description page of Project Settings.


#include "Transport.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

// Sets default values
ATransport::ATransport()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName("Interactive");
	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(RootComponent);
	Collider->SetCollisionProfileName("OverlapPawn");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ATransport::OnBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ATransport::OnEndOverlap);
	// Movement = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
	// Movement->MaxSpeed = 500;
	// Movement->MovementMode = EMovementMode::MOVE_Flying;
	// Movement->bAutoActivate = true;
}

// Called when the game starts or when spawned
void ATransport::BeginPlay()
{
	Super::BeginPlay();
	original_height = GetActorLocation().Z;
	if (MoveCurve)
	{
		FOnTimelineFloat MoveCallback;
		FOnTimelineEventStatic MoveFinishCallback;
		MoveCallback.BindUFunction(this, FName("LerpToMove"));
		MoveFinishCallback.BindUFunction(this, FName("MoveFinish"));
		MoveTimeline.AddInterpFloat(MoveCurve, MoveCallback);
		MoveTimeline.SetTimelineFinishedFunc(MoveFinishCallback);
		MoveTimeline.SetPlayRate(1.0f / Duration);
		MoveTimeline.SetNewTime(StartOffset);
		MoveTimeline.SetLooping(false);
		MoveTimeline.Stop();
	}
}

// Called every frame
void ATransport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// FVector location = GetActorLocation();
	// SetActorLocation(FVector(location.X, location.Y, original_height));
	if(isActive)
		MoveTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ATransport::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ATransport::OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ATP_ThirdPersonCharacter* player = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if (player) {
		// UE_LOG(LogTemp, Warning, TEXT("Collide"));

		// player->SetUpTransport(this);
		if(!isActive) MoveTimeline.PlayFromStart();
		isActive = true;
	}
}


void ATransport::OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ATP_ThirdPersonCharacter* player = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if (player) {
		// UE_LOG(LogTemp, Warning, TEXT("Leave"));
		// player->ClearTransport();
	}
}

void ATransport::LerpToMove(float value) {
	if (TrackRef) {
		USplineComponent* Spline = TrackRef->Spline;
		float length = Spline->GetSplineLength() * value;
		FVector pos = Spline->GetLocationAtDistanceAlongSpline(length, ESplineCoordinateSpace::Type::World);
		FRotator rot = Spline->GetRotationAtDistanceAlongSpline(length, ESplineCoordinateSpace::Type::World);
		SetActorLocationAndRotation(pos, rot);
	}
}

void ATransport::MoveFinish() {
	isActive = false;
}

void ATransport::Reset() {
	USplineComponent* Spline = TrackRef->Spline;
	FVector pos = Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::Type::World);
	SetActorLocation(pos);
	isActive = false;
}
