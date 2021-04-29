// Fill out your copyright notice in the Description page of Project Settings.


#include "HintVFX.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "LTGameInstance.h"

// Sets default values
AHintVFX::AHintVFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>("TriggerZone");
	TriggerZone->SetupAttachment(RootComponent);
	TriggerZone->SetCollisionProfileName("OverlapPawn");
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AHintVFX::OnBoxBeginOverlap);
	TriggerZone->OnComponentEndOverlap.AddDynamic(this, &AHintVFX::OnBoxEndOverlap);

	Text = CreateDefaultSubobject<UTextRenderComponent>("Text");
	Text->SetupAttachment(RootComponent);
	Text->SetHiddenInGame(true);

	TextCHS = CreateDefaultSubobject<UTextRenderComponent>("TextCHS");
	TextCHS->SetupAttachment(RootComponent);
	TextCHS->SetHiddenInGame(true);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	Particle->SetupAttachment(Text);
	Particle->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AHintVFX::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHintVFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHintVFX::OnBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATP_ThirdPersonCharacter>(OtherActor))
	{
		ULTGameInstance* gameIns = Cast<ULTGameInstance>(GetGameInstance());
		if (gameIns)
		{
			TEnumAsByte<DialogueVer> language = gameIns->GetActiveLanguage();
			 switch (language)
			{
			 case DialogueVer::English:
				 Text->SetHiddenInGame(false);
				 break;
			 case DialogueVer::Chinese:
				 TextCHS->SetHiddenInGame(false);
				 break;
			 default:
				 break;
			}
		}
		Particle->SetActive(true);
	}
}

void AHintVFX::OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ATP_ThirdPersonCharacter>(OtherActor))
	{
		Particle->SetActive(false);
		Text->SetHiddenInGame(true);
		TextCHS->SetHiddenInGame(true);
	}
}

