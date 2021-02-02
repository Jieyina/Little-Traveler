// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hook.generated.h"

UCLASS()
class LITTLETRAVELER_API AHook : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Hook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* End;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCableComponent* Cable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* Constraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTimelineComponent* ShootTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTimelineComponent* PullTimeline;

	FVector startPos;
	FVector endPos;
	UPROPERTY()
		class ATP_ThirdPersonCharacter* Player;

protected:
	UPROPERTY(EditAnywhere, Category = "Hook")
		float stopHeight;

	UPROPERTY(EditAnywhere, Category = "Hook")
		float stopDis;

	UPROPERTY(EditAnywhere, Category = "Hook")
		float swingForce;

	UPROPERTY(EditAnywhere, Category = "Hook")
		float shrinkRate;

	UPROPERTY(EditAnywhere, Category = "Hook")
		UCurveFloat* ShootCurve;

	UPROPERTY(EditAnywhere, Category = "Hook")
		UCurveFloat* PullCurve;
	
public:	
	// Sets default values for this actor's properties
	AHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void LerpToShoot(float value);
	UFUNCTION()
		void OnShootFinish();
	UFUNCTION()
		void LerpToPull(float value);
	UFUNCTION()
		void OnPullFinish();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Launch(FVector start, FVector end, ATP_ThirdPersonCharacter* player);
	void Swing(FVector dir);
	void ChangeRopeLength(float axisVal);
	void Release();

	FORCEINLINE class UStaticMeshComponent* GetHook() const { return Hook; }
	FORCEINLINE class UStaticMeshComponent* GetEnd() const { return End; }
};
