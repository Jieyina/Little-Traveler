// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
//#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

#include "../Line.h"
#include "../CPP_Pushable.h"
#include "../CPP_Climbable.h"
#include "../Rock_Climbable.h"
#include "../RockClimbTrigger_Start.h"
#include "../Faucet.h"
#include "../Collectable.h"
#include "../GameSave.h"
#include "../JumpableNode.h"
#include "../Hook.h"

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.05f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	GlideEquip = CreateDefaultSubobject<UStaticMeshComponent>("GlideEquip");
	GlideEquip->SetupAttachment(RootComponent);
	GlideEquip->SetCollisionProfileName("NoCollision");
	GlideEquip->SetHiddenInGame(true);

	MoveToPushTimeline = CreateDefaultSubobject<UTimelineComponent>("PushTimeline");
	MoveToClimbTimeline = CreateDefaultSubobject<UTimelineComponent>("ClimbTimeline");
	JumpEdgeTimeline = CreateDefaultSubobject<UTimelineComponent>("JumpEdgeTimeline");
	RockClimbTimeline = CreateDefaultSubobject<UTimelineComponent>("RockClimbTimeline");

	curLevel = 0;
	curEuip = EuipItem::FlourBomb;

	traceDistance = 30.0f;

	jumping = false;
	pushSpeed = 1.0f;
	pushing = false;
	pushDir = FVector(0, 0, 0);
	prevPos = FVector(0, 0, 0);
	prevRot = FRotator(0, 0, 0);

	gliding = false;
	canGlide = false;
	glideGravity = 0.15f;

	AxisForward = 0;
	AxisRight = 0;
	climbing = false;
	climbSpeed = 0.2f;
	climbDirV = FVector(0, 0, 0);
	climbDirH = FVector(0, 0, 0);
	climbDirF = FVector(0, 0, 0);
	climbStart = FVector(0, 0, 0);
	climbEnd = FVector(0, 0, 0);
	climbHit = FVector(0, 0, 0);
	climbRot = FRotator(0, 0, 0);
	climbLedge = false;
	jumpLedge = false;
	climbPoint = ClimbPoint::None;
	jumpDir = JumpDir::NotJump;

	canClimbRock = false;
	canClimbJump = false;
	rockClimbing = false;
	hangRock = false;
	rockClimbEnd = false;
	rockClimbPos = FVector(0, 0, 0);
	rockClimbRot = FRotator(0, 0, 0);

	shooting = false;
	pulled = false;
	swing = false;
	hookDis = 100.0f;
	launchRate = 1.5f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATP_ThirdPersonCharacter::OnJumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATP_ThirdPersonCharacter::OnJumpReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_ThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_ThirdPersonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATP_ThirdPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATP_ThirdPersonCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("AdjustRope", this, &ATP_ThirdPersonCharacter::AdjustRope);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATP_ThirdPersonCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATP_ThirdPersonCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATP_ThirdPersonCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATP_ThirdPersonCharacter::Interact);
	PlayerInputComponent->BindAction("SwitchEuip", IE_Pressed, this, &ATP_ThirdPersonCharacter::SwitchEuip);
	PlayerInputComponent->BindAction("UseEuip", IE_Pressed, this, &ATP_ThirdPersonCharacter::UseEuip);
	//PlayerInputComponent->BindAction("Glide", IE_Pressed, this, &ATP_ThirdPersonCharacter::Glide);
	//PlayerInputComponent->BindAction("Bomb", IE_Pressed, this, &ATP_ThirdPersonCharacter::UseFlourBomb);
	//PlayerInputComponent->BindAction("Hook", IE_Pressed, this, &ATP_ThirdPersonCharacter::Hook);
}


void ATP_ThirdPersonCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATP_ThirdPersonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ATP_ThirdPersonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ATP_ThirdPersonCharacter::OnJumpPressed()
{
	if (canClimbJump)
	{
		canClimbJump = false;
		if (RockClimbObj)
		{
			CurRock = RockClimbObj;
			hangRock = false;
			rockClimbPos = RockClimbObj->GetActorLocation() + offsets;
			rockClimbRot = RockClimbObj->GetActorRotation();
			prevPos = this->GetActorLocation();
			prevRot = this->GetActorRotation();
			if (!Cast<ARock_Climbable>(RockClimbObj))
				rockClimbEnd = true;
			rockClimbing = true;
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
			RockClimbTimeline->PlayFromStart();
		}
		return;
	}
	if (canJumpNode) 
	{
		if (NextNode) {
			canJumpNode = false;
			isJumpingNode = true;
			isOnNode = false;
			JumpableNodePosition = NextNode->GetActorLocation() + NextNode->PlayerOffsets;
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
		}
		return;
	}
	if (canGlide && GetCharacterMovement()->IsFalling())
	{
		gliding = true;
		canGlide = false;
		GlideEquip->SetHiddenInGame(false);
		LaunchCharacter(FVector(0, 0, 5), false, true);
		GetCharacterMovement()->GravityScale = glideGravity;
		GetCharacterMovement()->AirControl = 1.0f;
		UpdateGlideUI(false);
		return;
	}
	if (rockClimbing || hangRock || isJumpingNode || pulled || swing)
		return;
	if (pushing)
		StopPush();
	if (climbing)
		StopClimb();
	if (gliding)
		StopGlide();
	Jump();
	jumping = true;
}

void ATP_ThirdPersonCharacter::OnJumpReleased()
{
	StopJumping();
	jumping = false;
}

void ATP_ThirdPersonCharacter::TurnAtRate(float Rate)
{
	if(canMove)
	// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::LookUpAtRate(float Rate)
{
	if(canMove)
	// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::MoveForward(float Value)
{
	if (Transport && Value > 0.0f)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Move"));
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Transport->AddMovementInput(Direction, Value);
		// UE_LOG(LogTemp, Warning, TEXT("%s"),*Direction.ToString());

		// AddMovementInput(Direction, Value * Transport->Speed);
		return;
	}
	if (!isOnNode && canMove && Controller != NULL)
	{
		if (hangRock || pulled)
			return;
		if (pushing)
		{
			if (PushObj && PushObj->GetMoving() && Value > 0.0f)
				AddMovementInput(pushDir, Value * pushSpeed);
			return;
		}
		if (climbing)
		{
			AxisForward = UKismetMathLibrary::Round(Value);
			if (Value != 0.0f)
				Climb(true, Value);
			return;
		}
		if (swing && Value != 0.0f)
		{
			//FVector hookProj = UKismetMathLibrary::ProjectVectorOnToPlane(HookObj->GetEnd()->GetForwardVector(), FVector(0, 0, 1));
			//FVector camFw = UKismetMathLibrary::GetForwardVector(FRotator(0, Controller->GetControlRotation().Yaw, 0));
			//float angle = UKismetMathLibrary::SignOfFloat(hookProj.X * camFw.Y - camFw.X * hookProj.Y) * 
			//	UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(hookProj, camFw));
			//GetCapsuleComponent()->SetRelativeRotation(FRotator(0, angle, 0));
			//HookObj->Swing(camFw * Value);
			HookObj->Swing(Value);
			return;
		}
		if (Value != 0.0f)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void ATP_ThirdPersonCharacter::MoveRight(float Value)
{
	if (Transport && Value > 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		Transport->AddMovementInput(Direction, Value);
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *Direction.ToString());
		// AddMovementInput(FVector(Direction.X,, Value * Transport->Speed);
		return;
	}
	if (!isOnNode && canMove && Controller != NULL)
	{
		if (hangRock || pushing || pulled || swing)
			return;
		if (climbing)
		{
			AxisRight = UKismetMathLibrary::Round(Value);
			if (Value != 0.0f)
				Climb(false, Value);
			return;
		}
		//if (swing && Value != 0.0f)
		//{
		//	FVector hookProj = UKismetMathLibrary::ProjectVectorOnToPlane(HookObj->GetEnd()->GetForwardVector(), FVector(0, 0, 1));
		//	FVector camR = UKismetMathLibrary::GetRightVector(FRotator(0, Controller->GetControlRotation().Yaw, 0));
		//	float angle = UKismetMathLibrary::SignOfFloat(hookProj.X * camR.Y - camR.X * hookProj.Y) *
		//		UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(hookProj, camR));
		//	GetCapsuleComponent()->SetRelativeRotation(FRotator(0, angle, 0));
		//	HookObj->Swing(camR * Value);
		//	return;
		//}
		if (Value != 0.0f)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
	}
}

void ATP_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALine::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		Line = Cast<ALine>(Actors[0]);
	}
	Actors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		PlayerStart = Cast<APlayerStart>(Actors[0]);
	}
	Actors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHook::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		HookObj = Cast<AHook>(Actors[0]);
	}

	if (MoveCurve)
	{
		FOnTimelineFloat MoveToPushCallback;
		FOnTimelineEventStatic MoveToPushFinishCallback;
		MoveToPushCallback.BindUFunction(this, FName("LerpToPush"));
		MoveToPushFinishCallback.BindUFunction(this, FName("OnLerpToPushFinish"));
		MoveToPushTimeline->AddInterpFloat(MoveCurve, MoveToPushCallback);
		MoveToPushTimeline->SetTimelineFinishedFunc(MoveToPushFinishCallback);
		FOnTimelineFloat MoveToClimbCallback;
		FOnTimelineEventStatic MoveToClimbFinishCallback;
		MoveToClimbCallback.BindUFunction(this, FName("LerpToClimb"));
		MoveToClimbFinishCallback.BindUFunction(this, FName("OnLerpToClimbFinish"));
		MoveToClimbTimeline->AddInterpFloat(MoveCurve, MoveToClimbCallback);
		MoveToClimbTimeline->SetTimelineFinishedFunc(MoveToClimbFinishCallback);
	}

	if (JumpEdgeCurve)
	{
		FOnTimelineFloat JumpEdgeCallback;
		FOnTimelineEventStatic JumpEdgeFinishCallback;
		JumpEdgeCallback.BindUFunction(this, FName("LerpToJump"));
		JumpEdgeFinishCallback.BindUFunction(this, FName("OnLerpToJumpFinish"));
		JumpEdgeTimeline->AddInterpFloat(JumpEdgeCurve, JumpEdgeCallback);
		JumpEdgeTimeline->SetTimelineFinishedFunc(JumpEdgeFinishCallback);
	}

	if (ClimbRockCurve)
	{
		FOnTimelineFloat ClimbRockCallback;
		FOnTimelineEventStatic ClimbRockFinishCallback;
		ClimbRockCallback.BindUFunction(this, FName("ClimbJump"));
		ClimbRockFinishCallback.BindUFunction(this, FName("OnClimbJumpFinish"));
		RockClimbTimeline->AddInterpFloat(ClimbRockCurve, ClimbRockCallback);
		RockClimbTimeline->SetTimelineFinishedFunc(ClimbRockFinishCallback);
	}

	euipItems.Add(EuipItem::FlourBomb);
	if (curLevel > 1)
		euipItems.Add(EuipItem::Hook);
	if (curLevel > 2)
		euipItems.Add(EuipItem::BubbleWand);

	initAirContol = GetCharacterMovement()->AirControl;
	initGravity = GetCharacterMovement()->GravityScale;
	InitUI();
}

// Called every frame
void ATP_ThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//MoveToPushTimeline.TickTimeline(DeltaTime);
	//MoveToClimbTimeline.TickTimeline(DeltaTime);
	//JumpEdgeTimeline.TickTimeline(DeltaTime);
	//RockClimbTimeline.TickTimeline(DeltaTime);
	if (isTurningToForward) {
		GetCameraBoom()->bUsePawnControlRotation = false;
		canMove = false;
		FRotator delta = GetActorRotation() - forward;
		if (FMath::Abs(delta.Pitch) < 0.1f && FMath::Abs(delta.Roll) < 0.1f && FMath::Abs(delta.Yaw) < 0.1f) {
			// UE_LOG(LogTemp, Warning, TEXT("OVER"));
			isTurningToForward = false;
		}
		// UE_LOG(LogTemp, Warning, TEXT("SETTING %s"), *(GetActorRotation().ToString()));

		SetActorRotation(FMath::Lerp(FQuat(GetActorRotation()), FQuat(forward), 0.05f));
		if (FMath::Abs(GetCameraBoom()->TargetArmLength - climbingLength) > 1.f) {
			float length = FMath::Lerp(GetCameraBoom()->TargetArmLength, climbingLength, 0.05f);
			GetCameraBoom()->TargetArmLength = length;
		}
		
		return;
	}
	if (isJumpingNode) {
		FVector curLocation = GetActorLocation();
		SetActorLocation(FMath::Lerp(curLocation, JumpableNodePosition, 0.5f));
		FVector delta = GetActorLocation() - JumpableNodePosition;
		if (FMath::Abs(delta.X) < 0.1f && FMath::Abs(delta.Y) < 0.1f && FMath::Abs(delta.Z) < 0.1f) {
			// UE_LOG(LogTemp, Warning, TEXT("OVER"));
			isJumpingNode = false;
			if (NextNode) isOnNode = true;
			else GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
		}
	}
	if (canClimbRock && initializeClimbing) {
		// prevLength = GetCameraBoom()->TargetArmLength;
		GetCameraBoom()->TargetArmLength = climbingLength;
		GetCameraBoom()->TargetOffset = CameraShift;
		canMove = false;
		initializeClimbing = false;
	}
	if (canClimbRock && !rockClimbing)
	{
		FVector startOffset(0, 0, 0);
		if (wallPosition.X != 0) {
			startOffset.X = wallPosition.X - GetActorLocation().X - 15;
		}else if (wallPosition.Y != 0) {
			startOffset.Y = wallPosition.Y - GetActorLocation().Y - 15;
		}else if (wallPosition.Z != 0) {
			startOffset.Z = wallPosition.Z - GetActorLocation().Z - 15;
		}

		// FVector start = this->GetActorLocation() + rayStartOffset + startOffset;
		// FRotator controlRot = GetControlRotation();
		// FVector end = start + controlRot.Vector() * climbRayLength;
		// if (Line)
			// UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
			// Line->SetActorLocationAndRotation(start, controlRot);
		// FHitResult hit;
		// TArray<AActor*> ignoreActors;

		AActor* temp_next = nullptr;
		if (Cast<ARockClimbTrigger_Start>(CurRock)) {
			temp_next = (Cast<ARockClimbTrigger_Start>(CurRock))->CheckNext();
		}
		else {
			temp_next = (Cast<ARock_Climbable>(CurRock))->CheckNext();
		}
		canClimbJump = (temp_next!=nullptr);
		if (canClimbJump)
		{
			if (temp_next != CurRock && temp_next != RockClimbObj) {
				RockClimbObj = temp_next;
				if (RockClimbObj)
				{
					ARock_Climbable* climbObj = Cast<ARock_Climbable>(RockClimbObj);
					if (climbObj)
						climbObj->TurnOnOutline();
				}
			}
		}
		else {
			if (RockClimbObj != NULL)
			{
				ARock_Climbable* climbObj = Cast<ARock_Climbable>(RockClimbObj);
				if (climbObj)
					climbObj->TurnOffOutline();
				RockClimbObj = NULL;
			}
		}
	}else if (!canClimbRock && !initializeClimbing) {
		// UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
		GetCameraBoom()->TargetArmLength = prevLength;
		GetCameraBoom()->TargetOffset = FVector(0, 0, 0);
		GetCameraBoom()->bUsePawnControlRotation = true;
		canMove = true;
		initializeClimbing = true;
	}
}

void ATP_ThirdPersonCharacter::TurnToForward(FVector p_Forward) {
	FVector temp = p_Forward;
	isTurningToForward = true;
	temp.Normalize();
	forward = temp.Rotation();
	prevLength = GetCameraBoom()->TargetArmLength;
	//UE_LOG(LogClass, Log, TEXT("This a testing statement. %s"), *forward.ToString());

	// SetActorRotation(p_Forward.Rotation());
}

void ATP_ThirdPersonCharacter::Glide()
{
	if (gliding)
	{
		StopGlide();
		return;
	}
	if (canGlide && GetCharacterMovement()->IsFalling())
	{
		gliding = true;
		canGlide = false;
		GlideEquip->SetHiddenInGame(false);
		LaunchCharacter(FVector(0, 0, 5), false, true);
		GetCharacterMovement()->GravityScale = glideGravity;
		GetCharacterMovement()->AirControl = 1.0f;
		UpdateGlideUI(false);
	}
}

void ATP_ThirdPersonCharacter::StopGlide()
{
	gliding = false;
	GlideEquip->SetHiddenInGame(true);
	GetCharacterMovement()->GravityScale = initGravity;
	GetCharacterMovement()->AirControl = initAirContol;
}

void ATP_ThirdPersonCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (gliding)
	{
		StopGlide();
		return;
	}
	if (GetCharacterMovement()->Velocity.Z < -460.0f)
	{
		this->Destroy();
		bool hasSave = UGameplayStatics::DoesSaveGameExist("save", 0);
		if (hasSave)
		{
			UGameSave* save = Cast<UGameSave>(UGameplayStatics::LoadGameFromSlot("save", 0));
			if (save && CharacterBP)
			{
				ATP_ThirdPersonCharacter* newCharacter = GetWorld()->SpawnActor<ATP_ThirdPersonCharacter>(CharacterBP, save->GetSpawnPos(), save->GetSpawnRot());
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(newCharacter);
			}
		}
		else if (PlayerStart && CharacterBP)
		{
			ATP_ThirdPersonCharacter* newCharacter = GetWorld()->SpawnActor<ATP_ThirdPersonCharacter>(CharacterBP, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(newCharacter);
		}
	}
}

void ATP_ThirdPersonCharacter::Interact()
{
	if (pushing)
	{
		StopPush();
		return;
	}
	if (climbing)
	{
		StopClimb();
		return;
	}
	FHitResult hit;
	TArray<AActor*> ignoreActors;
	FVector start = this->GetActorLocation();
	FVector end = start + this->GetActorForwardVector() * traceDistance;
	bool hitResult = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, 2.0f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ignoreActors, EDrawDebugTrace::None, hit, true);
	if (!hitResult)
		return;
	if (hit.Actor->ActorHasTag("Pushable"))
	{
		if (gliding)
			StopGlide();
		PushObj = Cast<ACPP_Pushable>(hit.Actor);
		if (PushObj)
			PushObj->GetPushSide(this);
	}
	else if (hit.Actor->ActorHasTag("Climbable"))
	{
		if (gliding)
			StopGlide();
		ACPP_Climbable* climbObj = Cast<ACPP_Climbable>(hit.Actor);
		if (climbObj)
		{
			climbHit = hit.ImpactPoint;
			StartClimb(climbObj);
		}
	}
	else if (hit.Actor->ActorHasTag("Paper"))
	{
		canGlide = true;
		UpdateGlideUI(true);
	}
	else if (hit.Actor->ActorHasTag("Faucet"))
	{
		AFaucet* faucet = Cast<AFaucet>(hit.Actor);
		if (faucet)
			faucet->Activate();
	}
	else if (hit.Actor->ActorHasTag("Talk"))
	{
		Talk(hit.GetActor());
	}
	else if (hit.Actor->ActorHasTag("Collectable"))
	{
		ACollectable* collectItem = Cast<ACollectable>(hit.Actor);
		if (collectItem)
		{
			AddToInventory(collectItem->GetType(), collectItem->GetName());
			collectItem->Destroy();
		}
	}
}

void ATP_ThirdPersonCharacter::LerpToPush(float value)
{
	if (PushStart != NULL)
	{
		FVector newLocation = FMath::Lerp(prevPos, PushStart->GetComponentLocation(), value);
		FRotator newRotation = FMath::Lerp(prevRot, PushStart->GetComponentRotation(), value);
		this->SetActorLocationAndRotation(newLocation, newRotation);
	}
}

void ATP_ThirdPersonCharacter::OnLerpToPushFinish()
{
	if (PushObj != NULL)
	{
		PushObj->AttachToPlayer();
		pushing = true;
	}
}

void ATP_ThirdPersonCharacter::StartPush(float friction, FVector direction, USceneComponent* start)
{
	pushSpeed = 1 - friction;
	pushDir = direction;
	PushStart = start;
	prevPos = this->GetActorLocation();
	prevRot = this->GetActorRotation();
	MoveToPushTimeline->PlayFromStart();
}

void ATP_ThirdPersonCharacter::StopPush(bool changeCollision)
{
	if (changeCollision && PushObj)
	{
		PushObj->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		PushObj->SetPushed(false);
		PushObj->SetMoving(false);
	}
	pushing = false;
}

void ATP_ThirdPersonCharacter::SwitchEuip()
{
	curEuip = euipItems[(curEuip + 1) % euipItems.Num()];
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("curEquip %d"), (int)curEuip));
}

void ATP_ThirdPersonCharacter::UseEuip()
{
	if (curEuip == EuipItem::FlourBomb)
	{
		UseFlourBomb();
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, "use flour bomb");
		return;
	}
	if (curEuip == EuipItem::Hook)
	{
		Hook();
		return;
	}
}

bool ATP_ThirdPersonCharacter::IsExceedEnd(FVector pos)
{
	return FVector::DotProduct(pos - climbEnd, climbDirV) >= 0;
}

bool ATP_ThirdPersonCharacter::IsExceedStart(FVector pos)
{
	return FVector::DotProduct(pos - climbStart, climbDirV) <= 0;
}

void ATP_ThirdPersonCharacter::Climb(bool vertical, float axisValue)
{
	if (vertical)
	{
		FVector climbDis = climbDirV * axisValue * climbSpeed;
		if (!IsExceedEnd(this->GetActorLocation() + climbDis))
		{
			if (!IsExceedStart(this->GetActorLocation() + climbDis))
				this->AddActorWorldOffset(climbDis);
		}
		else
		{
			if (axisValue > 0 && climbLedge && !jumpLedge)
			{
				jumpLedge = true;
				this->SetActorRotation(FRotator(0, climbRot.Yaw, 0));
				prevPos = this->GetActorLocation();
				jumpDir = JumpDir::Up;
				JumpEdgeTimeline->PlayFromStart();
			}
		}
	}
	else
	{
		FVector climbDis = climbDirH * axisValue * climbSpeed;
		FHitResult hit;
		TArray<AActor*> ignoreActors;
		FVector start = this->GetActorLocation() + climbDis - climbDirF;
		FVector end = start + climbDirF * 50;
		bool hitResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, 
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ignoreActors, EDrawDebugTrace::None, hit, true);
		//bool hitResult = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_GameTraceChannel1);
		if (hitResult && hit.Actor->ActorHasTag("Climbable"))
		{
			this->AddActorWorldOffset(climbDis);
			return;
		}
		start = this->GetActorLocation() - climbDirF * 10;
		end = start + climbDirH * axisValue * 50;
		hitResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ignoreActors, EDrawDebugTrace::None, hit, true);
		if (hitResult && hit.Actor->ActorHasTag("Climbable"))
		{
			ACPP_Climbable* climbObj = Cast<ACPP_Climbable>(hit.Actor);
			if (climbObj)
			{
				climbHit = hit.ImpactPoint;
				StartClimb(climbObj);
			}
		}
	}
}

void ATP_ThirdPersonCharacter::StartClimb(ACPP_Climbable* climbObj)
{
	climbLedge = climbObj->GetIsLedge();
	climbStart = climbObj->GetStart()->GetComponentLocation();
	climbEnd = climbObj->GetEnd()->GetComponentLocation();
	climbRot = climbObj->GetEnd()->GetComponentRotation();
	climbDirV = climbObj->GetEnd()->GetUpVector();
	climbDirH = climbObj->GetEnd()->GetRightVector();
	climbDirF = climbObj->GetEnd()->GetForwardVector();
	prevPos = this->GetActorLocation();
	prevRot = this->GetActorRotation();
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	if (IsExceedEnd(climbHit))
		climbPoint = ClimbPoint::End;
	else if (IsExceedStart(climbHit))
		climbPoint = ClimbPoint::Start;
	else
		climbPoint = ClimbPoint::Hit;
	MoveToClimbTimeline->PlayFromStart();
}

void ATP_ThirdPersonCharacter::StopClimb()
{
	this->SetActorRotation(FRotator(0, climbRot.Yaw, 0));
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	climbing = false;
}

void ATP_ThirdPersonCharacter::LerpToClimb(float value)
{
	FVector newLocation;
	switch (climbPoint)
	{
	case ClimbPoint::End:
		newLocation = FMath::Lerp(prevPos, climbEnd, value);
		break;
	case ClimbPoint::Start:
		newLocation = FMath::Lerp(prevPos, climbStart, value);
		break;
	case ClimbPoint::Hit:
		newLocation = FMath::Lerp(prevPos, climbHit, value);
		break;
	case ClimbPoint::None:
		return;
	}
	FRotator newRotation = FMath::Lerp(prevRot, climbRot, value);
	this->SetActorLocationAndRotation(newLocation, newRotation);
}

void ATP_ThirdPersonCharacter::OnLerpToClimbFinish()
{
	climbPoint = ClimbPoint::None;
	climbing = true;
}

void ATP_ThirdPersonCharacter::LerpToJump(float value)
{
	if (jumpDir == JumpDir::Up)
	{
		FVector newLocation = prevPos + FVector(0.0f, 0.0f, 15.0f);
		this->SetActorLocation(FMath::Lerp(prevPos, newLocation, value));
	}
	else if (jumpDir == JumpDir::Forward)
	{
		FVector newLocation = prevPos + this->GetActorForwardVector() * 25;
		this->SetActorLocation(FMath::Lerp(prevPos, newLocation, value));
	}
}

void ATP_ThirdPersonCharacter::OnLerpToJumpFinish()
{
	if (jumpDir == JumpDir::Up)
	{
		jumpDir = JumpDir::Forward;
		prevPos = prevPos + FVector(0.0f, 0.0f, 15.0f);
		JumpEdgeTimeline->PlayFromStart();
	}
	else if (jumpDir == JumpDir::Forward)
	{
		jumpDir = JumpDir::NotJump;
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
		jumpLedge = false;
		climbing = false;
	}
}

void ATP_ThirdPersonCharacter::ClimbJump(float value)
{
	FVector newLocation = FMath::Lerp(prevPos, rockClimbPos, value);
	FRotator newRotation = FMath::Lerp(prevRot, rockClimbRot, value);
	this->SetActorLocationAndRotation(newLocation, newRotation);
}

void ATP_ThirdPersonCharacter::OnClimbJumpFinish()
{
	rockClimbing = false;
	if (!rockClimbEnd)
		hangRock = true;
}

void ATP_ThirdPersonCharacter::SetUpTransport(ATransport* p_Transport) {
	Transport = p_Transport;
	// GetCharacterMovement()->GravityScale = 0;
	// GetCharacterMovement()->GravityScale = 0;
}

void ATP_ThirdPersonCharacter::ClearTransport() {
	Transport = nullptr;
	// GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
}

void ATP_ThirdPersonCharacter::UseFlourBomb() {
	if (FlourBombNum > 0) {
		UE_LOG(LogTemp, Warning, TEXT("Blocked"));

		FlourBombNum--;
		Block = true;
		GetWorldTimerManager().SetTimer(FlourBombTimer, this, &ATP_ThirdPersonCharacter::ClearFlourBomb, FlourBombDuration, false, FlourBombDuration);
	}
}

void ATP_ThirdPersonCharacter::ClearFlourBomb() {
	UE_LOG(LogTemp, Warning, TEXT("Clear"));
	Block = false;
}

void ATP_ThirdPersonCharacter::Hook()
{
	if (!swing)
	{
		if (gliding)
			StopGlide();
		TArray<FHitResult> hits;
		TArray<AActor*> ignoreActors;
		FVector start = this->GetActorLocation();
		bool hitResult = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), start, start, hookDis,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, ignoreActors, EDrawDebugTrace::ForDuration, hits, true);
		if (hitResult && HookObj)
		{
			float minDis = 9999;
			AActor* obj = nullptr;
			for (FHitResult hit : hits)
			{
				FVector pos = hit.Actor->GetActorLocation();
				float dis = UKismetMathLibrary::Vector_Distance(pos, start);
				if (dis <= minDis)
				{
					minDis = dis;
					obj = hit.GetActor();
				}
			}
			shooting = true;
			HookObj->Launch(obj, this);
		}
	}
	else
	{
		swing = false;
		GetCapsuleComponent()->SetWorldRotation(GetCapsuleComponent()->GetRelativeRotation());
		GetCapsuleComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		LaunchCharacter(HookObj->GetEnd()->GetComponentVelocity() * launchRate, false, false);
		HookObj->Release();
	}
}

void ATP_ThirdPersonCharacter::AdjustRope(float axisVal)
{
	if (swing && axisVal != 0)
	{
		HookObj->ChangeRopeLength(axisVal);
	}
}

void ATP_ThirdPersonCharacter::AddToInventory(TEnumAsByte<ECollectableType> type, FString name)
{
	switch (type)
	{
	case ECollectableType::Collect_MainQuest:
		if (questItems.Contains(name))
			questItems[name] += 1;
		else
			questItems.Emplace(name, 1);
		break;
	case ECollectableType::Collect_Treasure:
		if (treasures.Contains(name))
			treasures[name] += 1;
		else
			treasures.Emplace(name, 1);
		break;
	case ECollectableType::Collect_Resource:
		if (resources.Contains(name))
			resources[name] += 1;
		else
			resources.Emplace(name, 1);
		break;
	default:
		return;
	}
}

void ATP_ThirdPersonCharacter::Craft(FString name)
{
	for (FCraftItem item : CraftItems)
	{
		if (item.name == name)
		{
			for (auto pair : item.materials)
			{
				if (resources.FindRef(pair.Key) < pair.Value)
					return;
			}
			for (auto pair : item.materials)
			{
				resources[pair.Key] -= pair.Value;
			}
			craftedTools.Emplace(name);
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("craft %s"), *name));
		}
		return;
	}
}