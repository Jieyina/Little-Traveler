// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Language.h"
#include "OnlineStats.h"
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
		ULanguage* dialogue;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector spawnPos;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FRotator spawnRot;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int numWalnut;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int catPushedTimes;

protected:
	void QueryAchievements();
	void OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	void UpdateAchievementProgress(const FString& Id, float Percent);
	FOnlineAchievementsWritePtr AchievementsWriteObjectPtr;
	void OnWriteAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);

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

	inline ULanguage* GetDialogueAt() { return dialogue; }
	inline void SetDialogue(ULanguage* newLanguage) { dialogue = newLanguage; }
	//void CollectWalnut();
};