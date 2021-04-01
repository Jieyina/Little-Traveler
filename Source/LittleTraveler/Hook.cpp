// Fill out your copyright notice in the Description page of Project Settings.


#include "Hook.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Engine/Engine.h"
#include "Hookable.h"


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

	End = CreateDefaultSubobject<UStaticMeshComponent>("End");
	End->SetupAttachment(Hook);
	End->SetCollisionProfileName("OverlapAllDynamic");
	End->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	Cable = CreateDefaultSubobject<UCableComponent>("Cable");
	Cable->SetupAttachment(Hook);
	Cable->SetCollisionProfileName("NoCollision");
	Cable->AttachEndTo.ComponentProperty = FName("End");
	Cable->EndLocation = FVector(0, 0, 0);

	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("Constraint");
	Constraint->SetupAttachment(RootComponent);

	ShootTimeline = CreateDefaultSubobject<UTimelineComponent>("ShootTimeline");
	PullTimeline = CreateDefaultSubobject<UTimelineComponent>("PullTimeline");

	startPos = FVector(0, 0, 0);
	endPos = FVector(0, 0, 0);
	startRot = FRotator(0, 0, 0);
	desPos = FVector(0, 0, 0);
	desRot = FRotator(0, 0, 0);
	stopHeight = 20.0f;
	stopDis = 50.0f;
	swingForce = 100.0f;
	shrinkRate = 0.5f;

	swinging = false;
	swingAngle = 45.0f;
	swingPeriod = 0.5f;
	launchSpeed = 200.0f;
	handOffset = 100.0f;
}

// Called when the game starts or when spawned
void AHook::BeginPlay()
{
	Super::BeginPlay();

	swingSpeed = swingAngle / swingPeriod;

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
	float offset = swingSpeed * DeltaTime;
	if (swinging)
	{
		if (flip)
		{
			Hook->AddRelativeRotation(FRotator(-offset, 0, 0));
		}
		else
		{
			Hook->AddRelativeRotation(FRotator(offset, 0, 0));
		}
		swingCountDown -= DeltaTime;
		if (swingCountDown < 0)
		{
			flip = !flip;
			swingCountDown = swingPeriod * 2;
		}
	}
}

void AHook::Launch(AActor* hookedObj, ATP_ThirdPersonCharacter* player)
{
	HookedObj = Cast<AHookable>(hookedObj);
	if (HookedObj)
	{
		Player = player;
		startPos = Player->GetMesh()->GetSocketLocation(FName("Hand_R"));
		endPos = HookedObj->GetHookPoint()->GetComponentLocation();
		this->SetActorLocation(endPos);
		this->SetActorRotation(HookedObj->GetArrow()->GetComponentRotation());
		Hook->SetWorldLocation(startPos);
		End->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), FName("Hand_R"));
		this->SetActorHiddenInGame(false);
		ShootTimeline->PlayFromStart();
	}
}

void AHook::LerpToShoot(float value)
{
	Hook->SetWorldLocation(UKismetMathLibrary::VLerp(startPos, endPos, value));
}

bool AHook::BeforeHookPoint(FVector pos)
{
	return FVector::DotProduct(endPos - pos, Player->GetActorForwardVector()) >= 0;
}

void AHook::OnShootFinish()
{
	Player->SetShooting(false);
	Player->GetCharacterMovement()->StopMovementImmediately();
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Player->SetPulled(true);
	End->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	End->AttachToComponent(Hook, FAttachmentTransformRules::KeepWorldTransform, FName("None"));
	startPos = Player->GetActorLocation();
	End->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(startPos, endPos));
	if (BeforeHookPoint(startPos))
		End->AddLocalRotation(FRotator(-90, 0, 0));
	else
		End->AddLocalRotation(FRotator(90, 180, 0));
	startPos = End->GetComponentLocation();
	startRot = End->GetComponentRotation();
	Player->AttachToComponent(End, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, 
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), FName("None"));
	Player->AddActorLocalOffset(FVector(0, 0, -handOffset));
	//desPos = endPos + FVector(0, 0, -UKismetMathLibrary::Vector_Distance(startPos, endPos));
	desPos = endPos + FVector(0, 0, HookedObj->GetSwingLength() * -1);
	if (FVector::DotProduct(Player->GetActorForwardVector(), HookedObj->GetArrow()->GetForwardVector()) >= 0)
		desRot = FRotator(0, HookedObj->GetArrow()->GetComponentRotation().Yaw, 0);
	else
		desRot = FRotator(0, 180 + HookedObj->GetArrow()->GetComponentRotation().Yaw, 0);
	PullTimeline->PlayFromStart();
}

void AHook::LerpToPull(float value)
{
	End->SetWorldLocation(UKismetMathLibrary::VLerp(startPos, desPos, value));
	End->SetWorldRotation(UKismetMathLibrary::RLerp(startRot, desRot, value, true));
}

void AHook::OnPullFinish()
{
	Player->SetPulled(false);
	//End->SetSimulatePhysics(true);
	//End->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	//Constraint->SetConstrainedComponents(Hook, FName("None"), End, FName("None"));
	//Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Player->SetSwing(true);
	swinging = true;
	swingCountDown = swingPeriod;
	flip = false;
}

void AHook::Swing(float axisVal)
{
	if (Hook->GetComponentLocation().Z - End->GetComponentLocation().Z > stopHeight)
	{
		//if ((BeforeHookPoint(Player->GetActorLocation()) && axisVal < 0) || (!BeforeHookPoint(Player->GetActorLocation()) && axisVal > 0))
		End->AddForce(UKismetMathLibrary::SignOfFloat(FVector::DotProduct(Player->GetActorForwardVector(), HookedObj->GetArrow()->GetForwardVector())) *
			axisVal * swingForce * HookedObj->GetArrow()->GetForwardVector(), FName("None"), true);
	}
}

void AHook::ChangeRopeLength(float axisVal)
{
	float dis = UKismetMathLibrary::Vector_Distance(Hook->GetComponentLocation(), End->GetComponentLocation());
	if ((axisVal > 0 && dis < stopHeight) || (axisVal < 0 && dis > stopDis))
	{
		return;
	}
	Constraint->SetConstrainedComponents(nullptr, FName("None"), nullptr, FName("None"));
	End->AddWorldOffset(UKismetMathLibrary::Normal(Hook->GetComponentLocation() - End->GetComponentLocation()) * shrinkRate * axisVal);
	Constraint->SetConstrainedComponents(Hook, FName("None"), End, FName("None"));
}

void AHook::Release(bool destroyPlayer)
{
	if (destroyPlayer)
		Player = nullptr;
	else
	{
		FVector launchVelo = UKismetMathLibrary::SignOfFloat(FVector::DotProduct(Player->GetActorForwardVector(), HookedObj->GetArrow()->GetForwardVector()))
			* HookedObj->GetArrow()->GetForwardVector() * launchSpeed;
		launchVelo.Z = launchSpeed;
		Player->SetActorRotation(desRot);
		Player->LaunchCharacter(launchVelo, false, true);
	}
	swinging = false;
	this->SetActorHiddenInGame(true);
	Hook->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	End->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	//Constraint->SetConstrainedComponents(nullptr, FName("None"), nullptr, FName("None"));
	//End->SetSimulatePhysics(false);
}