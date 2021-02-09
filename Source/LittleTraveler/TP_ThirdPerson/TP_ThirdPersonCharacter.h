// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../LittleTravelerTypes.h"
#include "../Transport.h"
#include "TP_ThirdPersonCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCraftItem
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		FString name;
	UPROPERTY(EditAnywhere)
		TMap<FString, int32> materials;
};

UCLASS(config = Game)
class ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* GlideEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTimelineComponent* MoveToPushTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTimelineComponent* MoveToClimbTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTimelineComponent* JumpEdgeTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTimelineComponent* RockClimbTimeline;

	UPROPERTY()
		class APlayerStart* PlayerStart;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool jumping;

	UPROPERTY()
		class ACPP_Pushable* PushObj;
	UPROPERTY()
		class USceneComponent* PushStart;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool pushing;
	float pushSpeed;
	FVector pushDir;

	FVector prevPos;
	FRotator prevRot;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool gliding;
	bool canGlide;
	float initAirContol;
	float initGravity;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool climbing;

	FVector climbDirV;
	FVector climbDirH;
	FVector climbDirF;
	FVector climbStart;
	FVector climbEnd;
	FVector climbHit;
	FRotator climbRot;
	bool climbLedge;
	bool jumpLedge;
	enum ClimbPoint
	{
		None = 0,
		Start = 1,
		End = 2,
		Hit = 3,
	};
	ClimbPoint climbPoint;
	enum JumpDir
	{
		NotJump = 0,
		Up = 1,
		Forward = 2
	};
	JumpDir jumpDir;

	UPROPERTY()
		class ALine* Line;

	// Rock climbing
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AActor* RockClimbObj;
	UPROPERTY()
		AActor* CurRock;
	bool canClimbRock;
	bool canClimbJump;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool rockClimbing;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool hangRock;
	bool rockClimbEnd;
	FVector rockClimbPos;
	FRotator rockClimbRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector offsets = FVector(0.0f, -4.0f, 3.0f);
	float prevLength = 35.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float climbingLength = 65.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector CameraShift = FVector(0, 20, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool canMove = true;
	bool initializeClimbing = true;
	FVector wallPosition;
	// float climbRayLength = 40;
	// FVector rayStartOffset = FVector(0, 0, 5);
	bool isTurningToForward = 0;
	FRotator forward;

	// Normal jumpable node
	bool canJumpNode = false;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isJumpingNode = false;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isOnNode = false;
	UPROPERTY()
	class AJumpableNode* NextNode = nullptr;
	FVector JumpableNodePosition;

	// Vehicle
	ATransport* Transport = nullptr;

	// Flour bomb
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float FlourBombDuration = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float FlourBombRadius = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int FlourBombNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool Block = false;
	FTimerHandle FlourBombTimer;

	// Grappling Hook
	UPROPERTY()
		class AHook* HookObj;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool shooting;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool pulled;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool swing;

	TMap<FString, int32> resources;
	TMap<FString, int32> questItems;
	TMap<FString, int32> treasures;
	TArray<FString> craftedTools;

protected:
	UPROPERTY(EditAnywhere, Category = "BP Setting|Reference")
		UClass* CharacterBP;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Reference")
		UCurveFloat* MoveCurve;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Reference")
		UCurveFloat* JumpEdgeCurve;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Reference")
		UCurveFloat* ClimbRockCurve;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Interact")
		float traceDistance;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Glide")
		float glideGravity;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Climb")
		float climbSpeed;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Hook")
		float hookDis;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Hook")
		float launchRate;

	UPROPERTY(EditAnywhere, Category = "BP Setting|Craft")
		TArray<FCraftItem> CraftItems;

public:
	ATP_ThirdPersonCharacter();

	UPROPERTY(BlueprintReadOnly)
		int32 AxisForward;
	UPROPERTY(BlueprintReadOnly)
		int32 AxisRight;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	void OnJumpPressed();
	void OnJumpReleased();

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void Interact();
	void Glide();
	void StopGlide();

	void Climb(bool vertical, float axisValue);
	void StartClimb(class ACPP_Climbable* climbObj);
	void StopClimb();
	bool IsExceedEnd(FVector pos);
	bool IsExceedStart(FVector pos);

	void Hook();
	void AdjustRope(float axisVal);

	void AddToInventory(TEnumAsByte<ECollectableType> type, FString name);

	UFUNCTION(BlueprintImplementableEvent)
		void InitUI();
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateGlideUI(bool turnon);

	UFUNCTION()
		void LerpToPush(float value);
	UFUNCTION()
		void OnLerpToPushFinish();
	UFUNCTION()
		void LerpToClimb(float value);
	UFUNCTION()
		void OnLerpToClimbFinish();
	UFUNCTION()
		void LerpToJump(float value);
	UFUNCTION()
		void OnLerpToJumpFinish();
	UFUNCTION()
		void ClimbJump(float value);
	UFUNCTION()
		void OnClimbJumpFinish();

	UFUNCTION()
		void UseFlourBomb();
	UFUNCTION()
		void ClearFlourBomb();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;

	void StartPush(float friction, FVector direction, USceneComponent* start);
	void StopPush(bool changeCollision = true);

	UFUNCTION(BlueprintCallable)
		void Craft(FString name);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UStaticMeshComponent* GetGlideEquip() const { return GlideEquip; }
	FORCEINLINE class ALine* GetLine() const { return Line; }

	inline void SetCanClimbRock(bool value) { canClimbRock = value; }

	inline void SetWallPosition(FVector p_WallPosition) {
		wallPosition = p_WallPosition;
	}

	inline void SetClimbingLength(float p_Length) { climbingLength = p_Length; }
	inline void SetCameraShift(FVector p_Shift) { CameraShift = p_Shift; }
	inline void SetPlayerOffset(FVector p_Offset) { offsets = p_Offset; }
	// inline void SetRayLength(float p_Length) { climbRayLength = p_Length; }
	// inline void SetRayStartOffset(FVector p_Offset) { rayStartOffset = p_Offset; }
	inline void SetCanJumpNode(bool p_CanJumpNode) { canJumpNode = p_CanJumpNode; }
	inline void SetNextNode(class AJumpableNode* p_Next) { NextNode = p_Next; }
	inline void SetCurRock(AActor* p_Rock) { CurRock = p_Rock; }

	inline void SetShooting(bool val) { shooting = val; }
	inline void SetPulled(bool val) { pulled = val; }
	inline void SetSwing(bool val) { swing = val; }

	void TurnToForward(FVector p_Forward);

	void SetUpTransport(ATransport* p_Transport);
	void ClearTransport();
};