// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Line.generated.h"

UCLASS()
class LITTLETRAVELER_API ALine : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* Particle;

public:
	// Sets default values for this actor's properties
	ALine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USceneComponent* GetSceneRoot() const { return SceneRoot; }
	FORCEINLINE class UParticleSystemComponent* GetParticle() const { return Particle; }
};
