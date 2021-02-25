// Fill out your copyright notice in the Description page of Project Settings.


#include "HintVFX.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

// Sets default values
AHintVFX::AHintVFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>("TriggerZone");
	TriggerZone->SetupAttachment(RootComponent);
	TriggerZone->SetCollisionProfileName("OverlapPawn");
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AHintVFX::OnBoxBeginOverlap);
	TriggerZone->OnComponentEndOverlap.AddDynamic(this, &AHintVFX::OnBoxEndOverlap);

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	Particle->SetupAttachment(RootComponent);
	Particle->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void AHintVFX::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHintVFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHintVFX::OnBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATP_ThirdPersonCharacter>(OtherActor))
		Particle->SetActive(true);
}

void AHintVFX::OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Particle->SetActive(false);
}

