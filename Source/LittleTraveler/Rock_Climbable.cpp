// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock_Climbable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARock_Climbable::ARock_Climbable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;
	Mesh_Normal = CreateDefaultSubobject<UStaticMeshComponent>("Mesh_Normal");
	Mesh_Normal->SetupAttachment(RootComponent);
	Mesh_Normal->SetCollisionProfileName("IgnoreCamera");
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetCollisionProfileName("BlockClimbable");
	Mesh_Outline = CreateDefaultSubobject<UStaticMeshComponent>("Mesh_Outline");
	Mesh_Outline->SetupAttachment(RootComponent);
	Mesh_Outline->SetCollisionProfileName("NoCollision");
	Mesh_Outline->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void ARock_Climbable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARock_Climbable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARock_Climbable::TurnOnOutline()
{
	Mesh_Outline->SetHiddenInGame(false);
}

void ARock_Climbable::TurnOffOutline()
{
	Mesh_Outline->SetHiddenInGame(true);
}

AActor* ARock_Climbable::CheckNext() {
	for (TPair<FKey, AActor*> p : Nexts) {
		if (UGameplayStatics::GetPlayerController(this, 0)->IsInputKeyDown(p.Key)) {
			return p.Value;
		}
	}
	return nullptr;
}

