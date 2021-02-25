// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "Trampoline.generated.h"

/**
 * 
 */
UCLASS()
class LITTLETRAVELER_API ATrampoline : public AInteractable
{
	GENERATED_BODY()

public:
	ATrampoline();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		float multiple = 1.0f;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Box;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
