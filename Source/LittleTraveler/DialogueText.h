// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogueText.generated.h"

UENUM()
enum DialogueVer
{
	Chinese,
	English
};

UCLASS()
class LITTLETRAVELER_API ADialogueText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueText();

	UPROPERTY(EditAnywhere)
		TMap<TEnumAsByte<DialogueVer>, FString> mDialogueVers;
	UPROPERTY(EditAnywhere)
		TEnumAsByte<DialogueVer> mActiveVer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
		void LoadText();

};
