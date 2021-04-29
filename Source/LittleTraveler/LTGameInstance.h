// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LittleTravelerTypes.h"
#include "LTGameInstance.generated.h"

UENUM(BlueprintType)
enum DialogueVer
{
	English, Chinese
};

UCLASS()
class LITTLETRAVELER_API ULTGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int levelId;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int prevLevelId;

	//UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//	class ULanguage* dialogue;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector spawnPos;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FRotator spawnRot;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int numWalnut;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int numPantryItem;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int catPushedTimes;

	TMap<FString, int32> questItems;
	TMap<FString, int32> treasures;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<DialogueVer> activeVer;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<TEnumAsByte<DialogueVer>, class ULanguage*> dialogueVers;

private:
	UFUNCTION(BlueprintCallable)
		void SwitchLevel(int prevLevel, int newLevel);

public:
	ULTGameInstance();

	inline int GetLevelId() { return levelId; }
	inline void SetLevelId(int newIndex) { levelId = newIndex; }
	inline int GetPrevLevelId() { return prevLevelId; }
	inline void SetPrevLevelId(int newIndex) { prevLevelId = newIndex; }
	inline FVector GetSpawnPos() { return spawnPos; }
	inline void SetSpawnPos(FVector pos) { spawnPos = pos; }
	inline FRotator GetSpawnRot() { return spawnRot; }
	inline void SetSpawnRot(FRotator rot) { spawnRot = rot; }
	inline int GetWalnutNum() { return numWalnut; }
	inline void AddWalnutNum() { numWalnut++; }
	inline int GetPantryItemNum() { return numPantryItem; }
	inline void AddPantryItemNum() { numPantryItem++; }

	void AddToInventory(TEnumAsByte<ECollectableType> type, FString name);
	UFUNCTION(BlueprintCallable)
		int GetItemNum(TEnumAsByte<ECollectableType> type, FString name);

	//inline ULanguage* GetDialogueAt() { return dialogue; }
	//inline void SetDialogue(class ULanguage* newLanguage) { dialogue = newLanguage; }
	UFUNCTION(BlueprintCallable)
		void SetActiveLanguage(int index);
};