// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSave.generated.h"

/**
 * 
 */
UCLASS()
class LITTLETRAVELER_API UGameSave : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector spawnPos;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator spawnRot;

public:
	UGameSave();

	inline FVector GetSpawnPos() { return spawnPos; }
	inline void SetSpawnPos(FVector pos) { spawnPos = pos; }
	inline FRotator GetSpawnRot() { return spawnRot; }
	inline void SetSpawnPos(FRotator rot) { spawnRot = rot; }
};
