// Fill out your copyright notice in the Description page of Project Settings.


#include "Trampoline.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

ATrampoline::ATrampoline() {
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add("Trampoline");

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetRelativeLocation(FVector(0, 0, 0));
	Box->SetCollisionProfileName("OverlapPawn");
	Box->OnComponentBeginOverlap.AddDynamic(this, &ATrampoline::OnOverlapBegin);
}

void ATrampoline::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (Cast<ATP_ThirdPersonCharacter>(OtherActor)) {
		ATP_ThirdPersonCharacter* character = Cast<ATP_ThirdPersonCharacter>(OtherActor);
		FVector cur_vel = character->GetVelocity();
		FVector bounce_vel(0.5f * cur_vel.X, 0.5f * cur_vel.Y, -cur_vel.Z * multiple);
		character->LaunchCharacter(bounce_vel, true, true);
	}
}