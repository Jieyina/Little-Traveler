// Fill out your copyright notice in the Description page of Project Settings.


#include "Trampoline.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Engine/Engine.h"

ATrampoline::ATrampoline() {
	PrimaryActorTick.bCanEverTick = true;

	hit = false;
	//this->Tags.Add("Trampoline");
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	RootComponent = SkeletalMesh;
	SkeletalMesh->SetCollisionProfileName("BlockAll");
	Shade = CreateDefaultSubobject<USkeletalMeshComponent>("Shade");
	Shade->SetupAttachment(RootComponent);
	Shade->SetCollisionProfileName("NoCollision");
	Shade->SetHiddenInGame(true);
	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(RootComponent);
	Collider->SetCollisionProfileName("OverlapPawn");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ATrampoline::OnSphereBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ATrampoline::OnSphereEndOverlap);
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(RootComponent);
	Box->SetCollisionProfileName("OverlapPawn");
	Box->OnComponentBeginOverlap.AddDynamic(this, &ATrampoline::OnBoxOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ATrampoline::OnBoxOverlapEnd);
}

void ATrampoline::OnSphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATP_ThirdPersonCharacter>(OtherActor))
		Shade->SetHiddenInGame(false);
}

void ATrampoline::OnSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Shade->SetHiddenInGame(true);
}

void ATrampoline::OnBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (Cast<ATP_ThirdPersonCharacter>(OtherActor)) {
		ATP_ThirdPersonCharacter* character = Cast<ATP_ThirdPersonCharacter>(OtherActor);
		hit = true;
		FVector cur_vel = character->GetVelocity();
		FVector bounce_vel(0.5f * cur_vel.X, 0.5f * cur_vel.Y, -cur_vel.Z * multiple);
		character->LaunchCharacter(bounce_vel, true, true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "launch");
	}
}

void ATrampoline::OnBoxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	hit = false;
}