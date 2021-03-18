// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractable::AInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName("Interactive");
	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(RootComponent);
	Collider->SetCollisionProfileName("OverlapPawn");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnSphereBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnSphereEndOverlap);
	ShadeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShadeMesh");
	ShadeMesh->SetupAttachment(RootComponent);
	ShadeMesh->SetCollisionProfileName("NoCollision");
	ShadeMesh->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::OnSphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		ShadeMesh->SetHiddenInGame(false);
}

void AInteractable::OnSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ShadeMesh->SetHiddenInGame(true);
}
