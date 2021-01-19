// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Pushable.h"
#include "Engine/Engine.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACPP_Pushable::ACPP_Pushable()
{
	PrimaryActorTick.bCanEverTick = true;
	pushedSide = -1;
	prevAngle = 0;
	prevZ = 0;
	moving = false;
	pushed = false;
	friction = 0.8f;

	this->Tags.Add("Pushable");

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(-130, 0, 0));
	Arrow1 = CreateDefaultSubobject<UArrowComponent>("Arrow1");
	Arrow1->SetupAttachment(RootComponent);
	Arrow1->SetRelativeLocationAndRotation(FVector(0, -130, 0), FRotator(0, 90, 0));
	Arrow2 = CreateDefaultSubobject<UArrowComponent>("Arrow2");
	Arrow2->SetupAttachment(RootComponent);
	Arrow2->SetRelativeLocationAndRotation(FVector(130, 0, 0), FRotator(0, 180, 0));
	Arrow3 = CreateDefaultSubobject<UArrowComponent>("Arrow3");
	Arrow3->SetupAttachment(RootComponent);
	Arrow3->SetRelativeLocationAndRotation(FVector(0, 130, 0), FRotator(0, -90, 0));
	TiltPoint = CreateDefaultSubobject<USceneComponent>("TiltPoint");
	TiltPoint->SetupAttachment(Arrow);
	TiltPoint->SetRelativeLocation(FVector(180, 0, -50));
	TiltPoint1 = CreateDefaultSubobject<USceneComponent>("TiltPoint1");
	TiltPoint1->SetupAttachment(Arrow1);
	TiltPoint1->SetRelativeLocation(FVector(180, 0, -50));
	TiltPoint2 = CreateDefaultSubobject<USceneComponent>("TiltPoint2");
	TiltPoint2->SetupAttachment(Arrow2);
	TiltPoint2->SetRelativeLocation(FVector(180, 0, -50));
	TiltPoint3 = CreateDefaultSubobject<USceneComponent>("TiltPoint3");
	TiltPoint3->SetupAttachment(Arrow3);
	TiltPoint3->SetRelativeLocation(FVector(180, 0, -50));
	Start = CreateDefaultSubobject<USceneComponent>("Start");
	Start->SetupAttachment(Arrow);
	Start->SetRelativeLocation(FVector(54, 0, 0));
	Start1 = CreateDefaultSubobject<USceneComponent>("Start1");
	Start1->SetupAttachment(Arrow1);
	Start1->SetRelativeLocation(FVector(54, 0, 0));
	Start2 = CreateDefaultSubobject<USceneComponent>("Start2");
	Start2->SetupAttachment(Arrow2);
	Start2->SetRelativeLocation(FVector(54, 0, 0));
	Start3 = CreateDefaultSubobject<USceneComponent>("Start3");
	Start3->SetupAttachment(Arrow3);
	Start3->SetRelativeLocation(FVector(54, 0, 0));
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Arrow);
	Box->SetRelativeLocation(FVector(50, 0, 0));
	Box->SetCollisionProfileName("OverlapPawn");
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Pushable::OnBoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACPP_Pushable::OnBoxEndOverlap);
	Box1 = CreateDefaultSubobject<UBoxComponent>("Box1");
	Box1->SetupAttachment(Arrow1);
	Box1->SetRelativeLocation(FVector(50, 0, 0));
	Box1->SetCollisionProfileName("OverlapPawn");
	Box1->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Pushable::OnBox1BeginOverlap);
	Box1->OnComponentEndOverlap.AddDynamic(this, &ACPP_Pushable::OnBox1EndOverlap);
	Box2 = CreateDefaultSubobject<UBoxComponent>("Box2");
	Box2->SetupAttachment(Arrow2);
	Box2->SetRelativeLocation(FVector(50, 0, 0));
	Box2->SetCollisionProfileName("OverlapPawn");
	Box2->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Pushable::OnBox2BeginOverlap);
	Box2->OnComponentEndOverlap.AddDynamic(this, &ACPP_Pushable::OnBox2EndOverlap);
	Box3 = CreateDefaultSubobject<UBoxComponent>("Box3");
	Box3->SetupAttachment(Arrow3);
	Box3->SetRelativeLocation(FVector(50, 0, 0));
	Box3->SetCollisionProfileName("OverlapPawn");
	Box3->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Pushable::OnBox3BeginOverlap);
	Box3->OnComponentEndOverlap.AddDynamic(this, &ACPP_Pushable::OnBox3EndOverlap);

	direction = FVector(0, 0, 0);
	disToPlayer = FVector(0, 0, 0);
	tiltPointPos = FVector(0, 0, 0);
	rotateVec = FVector(0, 0, 0);
	rotateAxis = FVector(0, 0, 0);
}

void ACPP_Pushable::BeginPlay()
{
	Super::BeginPlay();
	if (TiltCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineCallback.BindUFunction(this, FName("TiltAroundPoint"));
		TimelineFinishedCallback.BindUFunction(this, FName("StopTilt"));
		TiltTimeline.AddInterpFloat(TiltCurve, TimelineCallback);
		TiltTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void ACPP_Pushable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TiltTimeline.TickTimeline(DeltaTime);
	if (moving && Player!=NULL)
	{
		FVector temptPos = Player->GetActorLocation() + disToPlayer;
		bool temptMove = this->SetActorLocation(FVector(temptPos.X, temptPos.Y, prevZ), true);
		if (!temptMove)
		{
			Player->StopPush();
		}
	}
}

void ACPP_Pushable::AttachToPlayer()
{
	if (Player != NULL)
	{
		Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		disToPlayer = this->GetActorLocation() - Player->GetActorLocation();
		prevZ = this->GetActorLocation().Z;
		moving = true;
	}
}

void ACPP_Pushable::GetPushSide(ATP_ThirdPersonCharacter* player)
{
	Player = player;
	switch (pushedSide)
	{
	case 0:
		direction = Arrow->GetForwardVector();
		Player->StartPush(friction, direction, Start);
		break;
	case 1:
		direction = Arrow1->GetForwardVector();
		Player->StartPush(friction, direction, Start1);
		break;
	case 2:
		direction = Arrow2->GetForwardVector();
		Player->StartPush(friction, direction, Start2);
		break;
	case 3:
		direction = Arrow3->GetForwardVector();
		Player->StartPush(friction, direction, Start3);
		break;
	default:
		return;
	}
	pushed = true;
}

void ACPP_Pushable::Tilt()
{
	switch (pushedSide)
	{
	case 0:
		tiltPointPos = TiltPoint->GetComponentLocation();
		rotateAxis = TiltPoint->GetRightVector();
		break;
	case 1:
		tiltPointPos = TiltPoint1->GetComponentLocation();
		rotateAxis = TiltPoint1->GetRightVector();
		break;
	case 2:
		tiltPointPos = TiltPoint2->GetComponentLocation();
		rotateAxis = TiltPoint2->GetRightVector();
		break;
	case 3:
		tiltPointPos = TiltPoint3->GetComponentLocation();
		rotateAxis = TiltPoint3->GetRightVector();
		break;
	default:
		return;
	}
	rotateVec = this->GetActorLocation() - tiltPointPos;
	moving = false;
	Mesh->SetCollisionProfileName("BlockAll");
	TiltTimeline.PlayFromStart();
}

void ACPP_Pushable::TiltAroundPoint(float angle)
{
	FVector newLocation = tiltPointPos + rotateVec.RotateAngleAxis(angle, rotateAxis);
	this->SetActorLocation(newLocation, true);
	this->AddActorWorldRotation(UKismetMathLibrary::RotatorFromAxisAndAngle(rotateAxis, angle - prevAngle), true);
	prevAngle = angle;
}

void ACPP_Pushable::StopTilt()
{
	pushed = false;
	if (Player != NULL)
		Player->StopPush(false);
}

void ACPP_Pushable::OnBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!pushed)
		pushedSide = 0;
}

void ACPP_Pushable::OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!pushed)
		pushedSide = -1;
}

void ACPP_Pushable::OnBox1BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!pushed)
		pushedSide = 1;
}

void ACPP_Pushable::OnBox1EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!pushed)
		pushedSide = -1;
}

void ACPP_Pushable::OnBox2BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!pushed)
		pushedSide = 2;
}

void ACPP_Pushable::OnBox2EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!pushed)
		pushedSide = -1;
}

void ACPP_Pushable::OnBox3BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!pushed)
		pushedSide = 3;
}

void ACPP_Pushable::OnBox3EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!pushed)
		pushedSide = -1;
}