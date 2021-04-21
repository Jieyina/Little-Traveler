
// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSwitch.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ACameraSwitch::ACameraSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->SetupAttachment(RootComponent);
	Collider->SetCollisionProfileName("OverlapPawn");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACameraSwitch::OnBoxBeginOverlap);
	//Collider->OnComponentEndOverlap.AddDynamic(this, &ACameraSwitch::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void ACameraSwitch::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACameraSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraSwitch::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!init)
	{
		Player = Cast<ATP_ThirdPersonCharacter>(OtherActor);
		if (Player)
		{
			APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			Player->DisableInput(controller);
			controller->SetViewTargetWithBlend(cameras[0], blendTimes[0]);
			GetWorldTimerManager().SetTimer(SwitchCamTimer, this, &ACameraSwitch::SwitchCamera, blendTimes[0], false);
			init = true;
		}
	}
}

void ACameraSwitch::SwitchCamera()
{
	curCameraId++;
	if (curCameraId == cameras.Num())
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Player->EnableInput(controller);
		controller->SetViewTargetWithBlend(Player, blendTimes[curCameraId]);
		return;
	}
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(cameras[curCameraId], blendTimes[curCameraId]);
	GetWorldTimerManager().SetTimer(SwitchCamTimer, this, &ACameraSwitch::SwitchCamera, blendTimes[curCameraId], false);
}
