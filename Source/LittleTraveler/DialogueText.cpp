// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueText.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

// Sets default values
ADialogueText::ADialogueText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mActiveVer = DialogueVer::English;
}

// Called when the game starts or when spawned
void ADialogueText::BeginPlay()
{
	Super::BeginPlay();
	// TODO: to be commented out
	LoadText();
}

void ADialogueText::LoadText() {
	//ULTGameInstance* gameIns = Cast<ULTGameInstance>(GetGameInstance());
	//if (gameIns)
	//	gameIns->SetDialogue(mDialogueVers[mActiveVer]);
}

void ADialogueText::SetActiveLanguage(int index) {
	mActiveVer = static_cast<DialogueVer>(index);
}

