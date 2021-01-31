// Fill out your copyright notice in the Description page of Project Settings.


#include "Faucet.h"
#include "Components/StaticMeshComponent.h"

AFaucet::AFaucet()
{
	lifting = false;
	WaterRaiseHeight = 23.5f;
	ItemRaiseHeight = 21.0f;

	Water = CreateDefaultSubobject<UStaticMeshComponent>("Water");
	Water->SetupAttachment(RootComponent);
	Water->SetCollisionProfileName("NoCollision");
	Water->SetHiddenInGame(true);

	LiftTimeline = CreateDefaultSubobject<UTimelineComponent>("LiftTimeline");

	this->Tags.Add("Faucet");
}

void AFaucet::BeginPlay()
{
	Super::BeginPlay();

	if (LiftCurve)
	{
		FOnTimelineFloat LiftCallback;
		LiftCallback.BindUFunction(this, FName("LiftWater"));
		LiftTimeline->AddInterpFloat(LiftCurve, LiftCallback);
	}
	for (int i = 0; i < SinkWater.Num(); i++)
	{
		waterPrevPos.Add(SinkWater[i]->GetActorLocation());
		waterDes.Add(waterPrevPos[i] + FVector(0, 0, WaterRaiseHeight));
	}
	for (int i = 0; i < Items.Num(); i++)
	{
		itemPrevPos.Add(Items[i]->GetActorLocation());
		itemDes.Add(itemPrevPos[i] + FVector(0, 0, ItemRaiseHeight));
	}
}

void AFaucet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//LiftTimeline.TickTimeline(DeltaTime);
}

void AFaucet::LiftWater(float value)
{
	for (int i = 0; i < SinkWater.Num(); i++)
		SinkWater[i]->SetActorLocation(FMath::Lerp(waterPrevPos[i], waterDes[i], value));
	for (int i = 0; i < Items.Num(); i++)
		Items[i]->SetActorLocation(FMath::Lerp(itemPrevPos[i], itemDes[i], value));
}

void AFaucet::Activate()
{
	if (!lifting)
	{
		lifting = true;
		Water->SetHiddenInGame(false);
		LiftTimeline->PlayFromStart();
	}
}