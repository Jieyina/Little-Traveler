// Fill out your copyright notice in the Description page of Project Settings.


#include "Walnut.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "LTGameInstance.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

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

	ShadeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShadeMesh");
	ShadeMesh->SetupAttachment(RootComponent);
	ShadeMesh->SetCollisionProfileName("NoCollision");
	ShadeMesh->SetHiddenInGame(true);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	Particle->SetupAttachment(RootComponent);
	Particle->SetAutoActivate(false);

	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(RootComponent);
	Collider->SetCollisionProfileName("OverlapPawn");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AWalnut::OnSphereBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AWalnut::OnSphereEndOverlap);

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

void AWalnut::OnSphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATP_ThirdPersonCharacter>(OtherActor))
		ShadeMesh->SetHiddenInGame(false);
}

void AWalnut::OnSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ShadeMesh->SetHiddenInGame(true);
}

void AWalnut::Collect(class ATP_ThirdPersonCharacter* player)
{
	Mesh->SetHiddenInGame(true);
	ShadeMesh->SetHiddenInGame(true);
	Particle->Activate();
	ULTGameInstance* gameIns = Cast<ULTGameInstance>(GetGameInstance());
	if (gameIns)
	{
		gameIns->AddWalnutNum();
		if (gameIns->GetWalnutNum() == 1)
			player->UnlockAchievement(FString("ACH_ONE_WALNUT"), 100.0f);
		else if (gameIns->GetWalnutNum() == 6)
			player->UnlockAchievement(FString("ACH_ALL_WALNUT"), 100.0f);
	}
	GetWorldTimerManager().SetTimer(VFXTimer, this, &AWalnut::DestroyItem, VFXDuration, false);
}

void AWalnut::DestroyItem()
{
	this->Destroy();
}
