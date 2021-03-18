// Fill out your copyright notice in the Description page of Project Settings.


#include "Walnut.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

// Sets default values
AWalnut::AWalnut()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VFXDuration = 1.0f;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("Interactive");

	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	Particle->SetupAttachment(RootComponent);
	Particle->SetAutoActivate(false);

	this->Tags.Add("Walnut");
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

void AWalnut::Collect()
{
	Mesh->SetHiddenInGame(true);
	Particle->Activate();
	GetWorldTimerManager().SetTimer(VFXTimer, this, &AWalnut::DestroyItem, VFXDuration, false);
}

void AWalnut::DestroyItem()
{
	this->Destroy();
}
