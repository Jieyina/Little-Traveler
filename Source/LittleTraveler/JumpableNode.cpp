// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpableNode.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

// Sets default values
AJumpableNode::AJumpableNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;
	Mesh_Normal = CreateDefaultSubobject<UStaticMeshComponent>("Mesh_Normal");
	Mesh_Normal->SetupAttachment(RootComponent);
	Mesh_Normal->SetCollisionProfileName("NoCollision");
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetCollisionProfileName("OverlapPawn");
	Mesh_Outline = CreateDefaultSubobject<UStaticMeshComponent>("Mesh_Outline");
	Mesh_Outline->SetupAttachment(RootComponent);
	Mesh_Outline->SetCollisionProfileName("NoCollision");
	Mesh_Outline->SetHiddenInGame(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AJumpableNode::OnColliderBeginOverlap);
}

// Called when the game starts or when spawned
void AJumpableNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumpableNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpableNode::OnColliderBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ATP_ThirdPersonCharacter* character = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	UE_LOG(LogClass, Log, TEXT("Inside."));
	if (character)
	{
		UE_LOG(LogClass, Log, TEXT("Collision."));
		Mesh_Outline->SetHiddenInGame(true);

		if (NextNode) {
			if (IsStart) {
				character->SetClimbingLength(CameraLength);
				character->SetUpNormalJump(FVector(1, 0, 0));
			}
			UE_LOG(LogClass, Log, TEXT("This a testing statement."));
			NextNode->Mesh_Outline->SetHiddenInGame(false);
			character->SetCanJumpNode(true);
		}
		else {
			character->SetCanJumpNode(false);
			character->ResetAfterNormalJump();
			// character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
		}

		character->SetNextNode(NextNode);
	}
}