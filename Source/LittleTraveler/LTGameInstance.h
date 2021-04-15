// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LITTLETRAVELER_API ULTGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int levelId;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int prevLevelId;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<int32, FString> dialogue;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector spawnPos;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FRotator spawnRot;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int numWalnut;


public:
	ULTGameInstance();

	inline int GetLevelId() { return levelId; }
	inline void SetLevelId(int newIndex) { levelId = newIndex; }
	inline int GetPrevLevelId() { return prevLevelId; }
	inline void SetPrevLevelId(int newIndex) { prevLevelId = newIndex; }
	inline FVector GetSpawnPos() { return spawnPos; }
	inline void SetSpawnPos(FVector pos) { spawnPos = pos; }
	inline FRotator GetSpawnRot() { return spawnRot; }
	inline void SetSpawnPos(FRotator rot) { spawnRot = rot; }
	inline int GetWalnutNum() { return numWalnut; }
	inline void AddWalnutNum() { numWalnut++; }

	UFUNCTION()
	FString GetDialogueAt(int32 dialogueId) { return dialogue[dialogueId]; }
	UFUNCTION()
	void SetDialogue(TMap<int32, FString> newDialogue) { dialogue = newDialogue; }
};