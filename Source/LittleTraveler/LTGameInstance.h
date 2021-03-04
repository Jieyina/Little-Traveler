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

public:
	ULTGameInstance();

	inline int GetLevelId() { return levelId; }
	inline void SetLevelId(int newIndex) { levelId = newIndex; }
	
};
