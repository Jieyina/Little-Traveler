// Fill out your copyright notice in the Description page of Project Settings.


#include "Line.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ALine::ALine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;
	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	Particle->SetupAttachment(RootComponent);

	this->SetActorHiddenInGame(true);
}

// Called when the game starts or when spawned
void ALine::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}