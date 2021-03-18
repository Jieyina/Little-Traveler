// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Walnut.generated.h"

UCLASS()
class LITTLETRAVELER_API AWalnut : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* Particle;

	FTimerHandle VFXTimer;
	
public:	
	// Sets default values for this actor's properties
	AWalnut();

	void Collect();

protected:
	UPROPERTY(EditAnywhere)
		float VFXDuration;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyItem();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
