// Fill out your copyright notice in the Description page of Project Settings.


#include "Walnut.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

// Sets default values
AWalnut::AWalnut()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	type = ECollectableType::Collect_Treasure;
	name = FString("walnut");
	VFXDuration = 1.0f;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	Particle->SetupAttachment(RootComponent);
	Particle->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AWalnut::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWalnut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWalnut::CollectWalnut()
{
	Mesh->SetHiddenInGame(true);
	ShadeMesh->SetHiddenInGame(true);
	Particle->Activate();
	GetWorldTimerManager().SetTimer(VFXTimer, this, &AWalnut::DestroyItem, VFXDuration, false);
}

void AWalnut::DestroyItem()
{
	this->Destroy();
}
