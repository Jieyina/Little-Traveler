// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "LittleTravelerTypes.h"
#include "Collectable.generated.h"

UCLASS()
class LITTLETRAVELER_API ACollectable : public AInteractable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Collectable")
	TEnumAsByte<ECollectableType> type;
	UPROPERTY(EditAnywhere, Category = "Collectable")
	FString name;

public:
	ACollectable();

public:
	FORCEINLINE TEnumAsByte<ECollectableType> GetType() { return type; }
	FORCEINLINE FString GetName() { return name; }
};
