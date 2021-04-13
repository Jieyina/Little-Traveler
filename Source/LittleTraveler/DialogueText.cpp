// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueText.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "LTGameInstance.h"

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
	TMap<int32, FString> Texts;

	FString directory = FPaths::GameContentDir();
	FString result;
	FString myFile = directory + "Texts/" + mDialogueVers[mActiveVer];
	UE_LOG(LogTemp, Warning, TEXT("%s"), *myFile);
	if (FFileHelper::LoadFileToString(result, *myFile)) {
		TArray<FString> lines;
		result.ParseIntoArray(lines, TEXT("\n"), true);
		for (int i = 0; i < lines.Num(); i++) {
			FString Left, Right;
			lines[i].Split(TEXT(" "), &Left, &Right);
			UE_LOG(LogTemp, Warning, TEXT("%d"), FCString::Atoi(*Left));
			int32 index = FCString::Atoi(*Left);
			Texts.Add(index, Right);
		}
		UE_LOG(LogTemp, Warning, TEXT("%d"), lines.Num());
	}
	Cast<ULTGameInstance>(GetGameInstance())->SetDialogue(Texts);
}

