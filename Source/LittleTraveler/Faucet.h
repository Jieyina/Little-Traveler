// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/TimelineComponent.h"
#include "Faucet.generated.h"

/**
 * 
 */
UCLASS()
class LITTLETRAVELER_API AFaucet : public AInteractable
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Water;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTimelineComponent* LiftTimeline;

	bool lifting;
	TArray<FVector> waterPrevPos;
	TArray<FVector> itemPrevPos;
	TArray<FVector> waterDes;
	TArray<FVector> itemDes;

protected:
	UPROPERTY(EditAnywhere, Category = "Faucet")
		TArray<AActor*> SinkWater;

	UPROPERTY(EditAnywhere, Category = "Faucet")
		TArray<AActor*> Items;

	UPROPERTY(EditAnywhere, Category = "Faucet")
		float WaterRaiseHeight;

	UPROPERTY(EditAnywhere, Category = "Faucet")
		float ItemRaiseHeight;

	UPROPERTY(EditAnywhere, Category = "Faucet")
		UCurveFloat* LiftCurve;

public:
	AFaucet();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void LiftWater(float value);

public:
	UFUNCTION(BlueprintCallable)
		void Activate();

	virtual void Tick(float DeltaTime) override;
};
