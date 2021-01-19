// Fill out your copyright notice in the Description page of Project Settings.


#include "RockClimbTrigger_Start.h"
#include "Components/ShapeComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Line.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


ARockClimbTrigger_Start::ARockClimbTrigger_Start()
{
	GetCollisionComponent()->SetCollisionProfileName("OverlapPawn");
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARockClimbTrigger_Start::OnColliderBeginOverlap);
}

void ARockClimbTrigger_Start::OnColliderBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATP_ThirdPersonCharacter* character = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if (character)
	{
		character->SetCanClimbRock(true);
		character->TurnToForward(Forward);
		character->SetWallPosition(WallPosition);
		if (character->GetLine())
			// UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
			// character->GetLine()->SetActorHiddenInGame(false);
			// character->GetLine()->GetParticle()->Activate(true);
			character->SetClimbingLength(CameraLength);
			character->SetCameraShift(CameraShift);
			character->SetPlayerOffset(PlayerOffsets);
			character->SetCurRock(this);
			// character->SetRayLength(RayLength);
			// character->SetRayStartOffset(RayStartOffset);
	}
}

AActor* ARockClimbTrigger_Start::CheckNext() {
	for (TPair<FKey, AActor*> p : Nexts) {
		if (UGameplayStatics::GetPlayerController(this, 0)->IsInputKeyDown(p.Key)) {
			return p.Value;
		}
	}
	return nullptr;
}

