// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraSwitch.generated.h"

UCLASS()
class LITTLETRAVELER_API ACameraSwitch : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Collider;

	UPROPERTY()
		class ATP_ThirdPersonCharacter* Player;
	FTimerHandle SwitchCamTimer;
	bool init;
	int curCameraId;

protected:
	UPROPERTY(EditAnywhere, Category = "SwitchCamera")
		TArray<class ACameraActor*> cameras;

	UPROPERTY(EditAnywhere, Category = "SwitchCamera")
		TArray<float> blendTimes;
	
public:	
	// Sets default values for this actor's properties
	ACameraSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//	void OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void SwitchCamera();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
