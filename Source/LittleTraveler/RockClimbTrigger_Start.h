// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "RockClimbTrigger_Start.generated.h"

/**
 * 
 */
UCLASS()
class LITTLETRAVELER_API ARockClimbTrigger_Start : public ATriggerBox
{
	GENERATED_BODY()

public:
	ARockClimbTrigger_Start();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UseBlend;

	UPROPERTY(BlueprintReadWrite)
		bool IsBlending;

	UPROPERTY(BlueprintReadWrite)
		bool HasBlended;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector WallPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector CameraShift = FVector(0, 20, 0);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CameraLength = 65.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector PlayerOffsets = FVector(0.0f, -4.0f, 3.0f);

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// float RayLength = 40;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// FVector RayStartOffset = FVector(0, 0, 5);

	AActor* CheckNext();

protected:
	UFUNCTION()
		void OnColliderBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TMap<FKey, AActor*> Nexts;
};