// Fill out your copyright notice in the Description page of Project Settings.


#include "LTGameInstance.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"

ULTGameInstance::ULTGameInstance()
{
	levelId = -1;
	prevLevelId = -1;
}

void ULTGameInstance::SwitchLevel(int prevLevel, int newLevel)
{
	prevLevelId = prevLevel;
	levelId = newLevel;
	//QueryAchievements();
	//if (levelId == 1)
	//	UnlockAchievement(FString("ACH_NEW_GAME"), 100.0f);
	//else if (levelId == 0)
	//{
	//	if (prevLevelId == 1)
	//		UnlockAchievement(FString("ACH_LEVEL_1"), 100.0f);
	//	else if (prevLevelId == 2)
	//		UnlockAchievement(FString("ACH_LEVEL_2.2"), 100.0f);
	//	else if (prevLevelId == 3)
	//		UnlockAchievement(FString("ACH_LEVEL_3"), 100.0f);
	//}
}

//void ULTGameInstance::CollectWalnut()
//{
//	numWalnut++;
//	if (numWalnut == 1)
//		UnlockAchievement(FString("ACH_ONE_WALNUT"), 100.0f);
//	else if (numWalnut >= 6)
//		UnlockAchievement(FString("ACH_ALL_WALNUT"), 100.0f);
//}

void ULTGameInstance::QueryAchievements()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
			IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface();
			if (Achievements.IsValid())
			{
				Achievements->QueryAchievements(*UserId, FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &ULTGameInstance::OnQueryAchievementsComplete));
				//Achievements->QueryAchievements(*UserId.Get());
			}
		}
	}
}

void ULTGameInstance::OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "achievements were cached");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "achievements cache failed");
	}
}

void ULTGameInstance::UpdateAchievementProgress(const FString& Id, float Percent)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
			IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface();
			if (Achievements.IsValid() && (!AchievementsWriteObjectPtr.IsValid() || !AchievementsWriteObjectPtr->WriteState != EOnlineAsyncTaskState::InProgress))
			{
				AchievementsWriteObjectPtr = MakeShareable(new FOnlineAchievementsWrite());
				AchievementsWriteObjectPtr->SetFloatStat(*Id, Percent);
				FOnlineAchievementsWriteRef AchievementsWriteObjectRef = AchievementsWriteObjectPtr.ToSharedRef();
				Achievements->WriteAchievements(*UserId, AchievementsWriteObjectRef, FOnAchievementsWritten::FDelegate::CreateUObject(this, &ULTGameInstance::OnWriteAchievementsComplete));
			}
		}
	}
}

void ULTGameInstance::OnWriteAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "achievements written");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "achievements write failed");
	}
}


