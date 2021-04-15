// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Language.generated.h"

/**
 * 
 */
UCLASS()
class LITTLETRAVELER_API ULanguage : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FString> Texts;
};
