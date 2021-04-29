// Fill out your copyright notice in the Description page of Project Settings.


#include "LTGameInstance.h"
#include "Language.h"

ULTGameInstance::ULTGameInstance()
{
	levelId = -1;
	prevLevelId = -1;
	activeVer = DialogueVer::English;
}

void ULTGameInstance::SwitchLevel(int prevLevel, int newLevel)
{
	prevLevelId = prevLevel;
	levelId = newLevel;
}

void ULTGameInstance::AddToInventory(TEnumAsByte<ECollectableType> type, FString name)
{
	switch (type)
	{
	case ECollectableType::Collect_MainQuest:
		if (questItems.Contains(name))
			questItems[name] += 1;
		else
			questItems.Emplace(name, 1);
		break;
	case ECollectableType::Collect_Treasure:
		if (treasures.Contains(name))
			treasures[name] += 1;
		else
			treasures.Emplace(name, 1);
		break;
	default:
		return;
	}
}

int ULTGameInstance::GetItemNum(TEnumAsByte<ECollectableType> type, FString name)
{
	switch (type)
	{
	case ECollectableType::Collect_MainQuest:
		return questItems.FindRef(name);
	case ECollectableType::Collect_Treasure:
		return treasures.FindRef(name);
	default:
		return 0;
	}
}

void ULTGameInstance::SetActiveLanguage(int index) {
	activeVer = static_cast<DialogueVer>(index);
}

