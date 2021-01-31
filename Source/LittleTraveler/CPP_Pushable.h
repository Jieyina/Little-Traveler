// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "CPP_Pushable.generated.h"

UCLASS()
class LITTLETRAVELER_API ACPP_Pushable : public AInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Arrow1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Arrow2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Arrow3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* TiltPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* TiltPoint1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* TiltPoint2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* TiltPoint3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Start;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Start1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Start2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Start3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTimelineComponent* TiltTimeline;

	UPROPERTY()
		class ATP_ThirdPersonCharacter* Player;
	int32 pushedSide;
	FVector disToPlayer;
	FVector tiltPointPos;
	FVector rotateVec;
	FVector rotateAxis;
	float prevAngle;
	float prevZ;
	bool moving;
	bool pushed;

protected:
	UPROPERTY(EditAnywhere, Category = "Push")
		float friction;

	UPROPERTY(EditAnywhere, Category = "Push")
		UCurveFloat* TiltCurve;

	UPROPERTY(BlueprintReadOnly)
		FVector direction;

public:
	ACPP_Pushable();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnBox1BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBox1EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnBox2BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBox2EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnBox3BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBox3EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void GetPushSide(ATP_ThirdPersonCharacter* player);
	void AttachToPlayer();

	UFUNCTION(BlueprintCallable)
		void Tilt();
	UFUNCTION()
		void TiltAroundPoint(float angle);
	UFUNCTION()
		void StopTilt();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UArrowComponent* GetArrow() const { return Arrow; }
	FORCEINLINE UArrowComponent* GetArrow1() const { return Arrow1; }
	FORCEINLINE UArrowComponent* GetArrow2() const { return Arrow2; }
	FORCEINLINE UArrowComponent* GetArrow3() const { return Arrow3; }
	FORCEINLINE USceneComponent* GetTiltPoint() const { return TiltPoint; }
	FORCEINLINE USceneComponent* GetTiltPoint1() const { return TiltPoint1; }
	FORCEINLINE USceneComponent* GetTiltPoint2() const { return TiltPoint2; }
	FORCEINLINE USceneComponent* GetTiltPoint3() const { return TiltPoint3; }
	FORCEINLINE USceneComponent* GetStart() const { return Start; }
	FORCEINLINE USceneComponent* GetStart1() const { return Start1; }
	FORCEINLINE USceneComponent* GetStart2() const { return Start2; }
	FORCEINLINE USceneComponent* GetStart3() const { return Start3; }
	FORCEINLINE UBoxComponent* GetBox() const { return Box; }
	FORCEINLINE UBoxComponent* GetBox1() const { return Box1; }
	FORCEINLINE UBoxComponent* GetBox2() const { return Box2; }
	FORCEINLINE UBoxComponent* GetBox3() const { return Box3; }

	inline FVector GetDirection() { return direction; }
	inline bool GetMoving() { return moving; }
	inline void SetMoving(bool val) { moving = val; }
	inline void SetPushed(bool val) { pushed = val; }
};
