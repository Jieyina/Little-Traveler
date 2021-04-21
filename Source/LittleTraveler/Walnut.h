// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "GameFramework/Actor.h"
#include "Walnut.generated.h"

UCLASS()
class LITTLETRAVELER_API AWalnut : public ACollectable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* Particle;

	FTimerHandle VFXTimer;
	
public:	
	// Sets default values for this actor's properties
	AWalnut();

	void CollectWalnut();

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
