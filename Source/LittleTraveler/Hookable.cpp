// Fill out your copyright notice in the Description page of Project Settings.


#include "Hookable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

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

