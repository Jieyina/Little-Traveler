// Fill out your copyright notice in the Description page of Project Settings.


#include "Hookable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHookable::AHookable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);

	ShadeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShadeMesh");
	ShadeMesh->SetupAttachment(RootComponent);
	ShadeMesh->SetCollisionProfileName("NoCollision");
	ShadeMesh->SetHiddenInGame(true);

	HookPoint = CreateDefaultSubobject<USceneComponent>("HookPoint");
	HookPoint->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<USphereComponent>("HookableRange");
	Collider->SetupAttachment(RootComponent);
	Collider->SetSphereRadius(100);
	Collider->SetCollisionProfileName("OverlapPawn");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AHookable::OnSphereBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AHookable::OnSphereEndOverlap);

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHookable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHookable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHookable::OnSphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		ShadeMesh->SetHiddenInGame(false);
}

void AHookable::OnSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ShadeMesh->SetHiddenInGame(true);
}

