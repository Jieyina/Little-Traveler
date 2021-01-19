// Fill out your copyright notice in the Description page of Project Settings.


#include "Transport.h"
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
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
	Movement->MaxSpeed = 500;
	// Movement->MovementMode = EMovementMode::MOVE_Flying;
	// Movement->bAutoActivate = true;
}

// Called when the game starts or when spawned
void ATransport::BeginPlay()
{
	Super::BeginPlay();
	original_height = GetActorLocation().Z;
}

// Called every frame
void ATransport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = GetActorLocation();
	SetActorLocation(FVector(location.X, location.Y, original_height));
}

// Called to bind functionality to input
void ATransport::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ATransport::OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ATP_ThirdPersonCharacter* player = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if (player) {
		UE_LOG(LogTemp, Warning, TEXT("Collide"));

		player->SetUpTransport(this);
	}
}


void ATransport::OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ATP_ThirdPersonCharacter* player = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if (player) {
		UE_LOG(LogTemp, Warning, TEXT("Leave"));
		player->ClearTransport();
	}
}
