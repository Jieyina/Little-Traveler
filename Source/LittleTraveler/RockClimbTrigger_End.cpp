// Fill out your copyright notice in the Description page of Project Settings.


#include "RockClimbTrigger_End.h"
#include "Components/ShapeComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Line.h"
#include "GameFramework/CharacterMovementComponent.h"

ARockClimbTrigger_End::ARockClimbTrigger_End()
{
	GetCollisionComponent()->SetCollisionProfileName("BlockClimbable");
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARockClimbTrigger_End::OnColliderBeginOverlap);
}

void ARockClimbTrigger_End::OnColliderBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATP_ThirdPersonCharacter* character = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if (character)
	{
		character->SetCanClimbRock(false);
		if (character->GetLine())
			character->GetLine()->SetActorHiddenInGame(true);
		character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	}
}
