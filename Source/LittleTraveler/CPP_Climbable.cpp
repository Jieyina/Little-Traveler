// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Climbable.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACPP_Climbable::ACPP_Climbable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->Tags.Add("Climbable");

	isLedge = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;
	Arrow = CreateDefaultSubobject <UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeRotation(FRotator(90, 0, 0));
	Start = CreateDefaultSubobject<USceneComponent>("Start");
	Start->SetupAttachment(RootComponent);
	Start->SetRelativeLocation(FVector(-3.0f, 0.0f, -20.0f));
	End = CreateDefaultSubobject<USceneComponent>("End");
	End->SetupAttachment(RootComponent);
	End->SetRelativeLocation(FVector(-3.0f, 0.0f, 30.0f));
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(RootComponent);
	Box->SetRelativeScale3D(FVector(0.1f, 1.0f, 1.0f));
	Box->SetCollisionProfileName("BlockInteractable");
}

// Called when the game starts or when spawned
void ACPP_Climbable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Climbable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

