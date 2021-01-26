// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Track.h"
#include "Transport.generated.h"

UCLASS()
class LITTLETRAVELER_API ATransport : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATransport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* Collider;

	bool isActive = false;
	FTimeline MoveTimeline;
	float LerpAlpha = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		UCurveFloat* MoveCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		ATrack* TrackRef;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		// class UFloatingPawnMovement* Movement;

	float original_height = 0;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void LerpToMove(float value);
	UFUNCTION()
		void MoveFinish();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		float Duration = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		float StartOffset = 0.5f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	inline void ToggleActive() { isActive = (!isActive); }

};
