// Fill out your copyright notice in the Description page of Project Settings.


#include "Hook.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

// Sets default values
AHook::AHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Hook = CreateDefaultSubobject<UStaticMeshComponent>("Hook");
	Hook->SetupAttachment(RootComponent);
	Hook->SetCollisionProfileName("OverlapAllDynamic");
	Hook->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	Cable = CreateDefaultSubobject<UCableComponent>("Cable");
	Cable->SetupAttachment(Hook);
	Cable->SetCollisionProfileName("NoCollision");
	Cable->AttachEndTo.ComponentProperty = FName("End");
	Cable->EndLocation = FVector(0, 0, 0);

	End = CreateDefaultSubobject<UStaticMeshComponent>("End");
	End->SetupAttachment(RootComponent);
	End->SetCollisionProfileName("OverlapAllDynamic");
	End->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("Constraint");
	Constraint->SetupAttachment(RootComponent);

	ShootTimeline = CreateDefaultSubobject<UTimelineComponent>("ShootTimeline");
	PullTimeline = CreateDefaultSubobject<UTimelineComponent>("PullTimeline");

	startPos = FVector(0, 0, 0);
	endPos = FVector(0, 0, 0);

	stopHeight = 20.0f;
	swingForce = 100.0f;
	shrinkRate = 0.5f;
}

// Called when the game starts or when spawned
void AHook::BeginPlay()
{
	Super::BeginPlay();

	if (ShootCurve)
	{
		FOnTimelineFloat ShootCallback;
		FOnTimelineEventStatic ShootFinishCallback;
		ShootCallback.BindUFunction(this, FName("LerpToShoot"));
		ShootFinishCallback.BindUFunction(this, FName("OnShootFinish"));
		ShootTimeline->AddInterpFloat(ShootCurve, ShootCallback);
		ShootTimeline->SetTimelineFinishedFunc(ShootFinishCallback);
	}

	if (PullCurve)
	{
		FOnTimelineFloat PullCallback;
		FOnTimelineEventStatic PullFinishCallback;
		PullCallback.BindUFunction(this, FName("LerpToPull"));
		PullFinishCallback.BindUFunction(this, FName("OnPullFinish"));
		PullTimeline->AddInterpFloat(PullCurve, PullCallback);
		PullTimeline->SetTimelineFinishedFunc(PullFinishCallback);
	}

	this->SetActorHiddenInGame(true);
}

// Called every frame
void AHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHook::Launch(FVector start, FVector end, ATP_ThirdPersonCharacter* player)
{
	this->SetActorLocation(end);
	Hook->SetWorldLocation(start);
	End->SetWorldLocation(start);
	this->SetActorHiddenInGame(false);
	startPos = start;
	endPos = end;
	Player = player;
	ShootTimeline->PlayFromStart();
}

void AHook::LerpToShoot(float value)
{
	Hook->SetWorldLocation(UKismetMathLibrary::VLerp(startPos, endPos, value));
}

void AHook::OnShootFinish()
{
	End->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(startPos, endPos));
	End->AddLocalRotation(FRotator(-90, 0, 0));
	Player->AttachToComponent(End, FAttachmentTransformRules(EAttachmentRule::KeepWorld, 
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), FName("None"));
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	PullTimeline->PlayFromStart();
}

void AHook::LerpToPull(float value)
{
	End->SetWorldLocation(UKismetMathLibrary::VLerp(startPos, endPos, value));
}

void AHook::OnPullFinish()
{
	End->SetSimulatePhysics(true);
	Constraint->SetConstrainedComponents(Hook, FName("None"), End, FName("None"));
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Player->SetSwing(true);
}

void AHook::Swing(FVector dir)
{
	if (Hook->GetComponentLocation().Z - End->GetComponentLocation().Z > stopHeight)
	{
		End->AddForce(dir * swingForce, FName("None"), true);
	}
}

void AHook::ChangeRopeLength(float axisVal)
{
	Constraint->SetConstrainedComponents(nullptr, FName("None"), nullptr, FName("None"));
	End->AddWorldOffset(UKismetMathLibrary::Normal(Hook->GetComponentLocation() - End->GetComponentLocation()) * shrinkRate * axisVal);
	Constraint->SetConstrainedComponents(Hook, FName("None"), End, FName("None"));
}

void AHook::Release()
{
	this->SetActorHiddenInGame(true);
	Hook->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	End->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	Constraint->SetConstrainedComponents(nullptr, FName("None"), nullptr, FName("None"));
	End->SetSimulatePhysics(false);
}