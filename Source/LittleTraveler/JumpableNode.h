// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpableNode.generated.h"

UCLASS()
class LITTLETRAVELER_API AJumpableNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpableNode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AJumpableNode* NextNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CameraLength = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector PlayerOffsets = FVector(0.0f, -4.0f, 3.0f);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh_Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh_Outline;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnColliderBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
