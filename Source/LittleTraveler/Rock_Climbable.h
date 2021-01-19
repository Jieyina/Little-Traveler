// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rock_Climbable.generated.h"

UCLASS()
class LITTLETRAVELER_API ARock_Climbable : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh_Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh_Outline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TMap<FKey, AActor*> Nexts;

public:	
	// Sets default values for this actor's properties
	ARock_Climbable();

	AActor* CheckNext();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TurnOnOutline();
	void TurnOffOutline();

	FORCEINLINE UStaticMeshComponent* GetMeshNormal() const { return Mesh_Normal; }
	FORCEINLINE USphereComponent* GetSphere() const { return Sphere; }
	FORCEINLINE UStaticMeshComponent* GetMeshOutline() const { return Mesh_Outline; }
};
