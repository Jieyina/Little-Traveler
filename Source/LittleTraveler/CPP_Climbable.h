// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Climbable.generated.h"

UCLASS()
class LITTLETRAVELER_API ACPP_Climbable : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Start;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* End;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box;
protected:
	UPROPERTY(EditAnywhere, Category = "Climb")
		bool isLedge;

public:	
	// Sets default values for this actor's properties
	ACPP_Climbable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE class USceneComponent* GetStart() const { return Start; }
	FORCEINLINE class USceneComponent* GetEnd() const { return End; }
	inline bool GetIsLedge() { return isLedge; }
};
