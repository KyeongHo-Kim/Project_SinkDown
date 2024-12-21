#include "SinkDownProject/Player/SinkDownProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"

#include "SinkDownProject/HealthSystem/HealthComponent.h"
#include "SinkDownProject/CombatSystem/CombatComponent.h"
//Interaction
#include "Components/BoxComponent.h"
#include "SinkDownProject/InteractionSystem/InteractionInterface.h"
//Weapon
#include "SinkDownProject/Items/Weapon/WeaponManager.h"
#include "SinkDownProject/DataAsset/WeaponData.h"
//Aiming
#include "Components/TimelineComponent.h"
#include "SinkDownProject/UI/CrosshairWidget.h"
//Skill
#include "SinkDownProject/SkillSystem/SkillManager.h"
#include "SinkDownProject/SkillSystem/SkillTypes.h"
//GameMode
#include "SinkDownProject/SinkDownProjectGameMode.h"
//MugunghwaGame
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaGameComponent.h"
//PlayerSound
#include "SinkDownProject/Player/SoundComponent.h"
// DiaryWidget
#include "SinkDownProject/UI/DiaryCollectionWidget.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);
ASinkDownProjectCharacter::ASinkDownProjectCharacter() 
	: Interface(nullptr)
	, InteractionCheckInterval(0.5f)
	, CurrentWeapon(nullptr)
	, WalkSpeed(300.0f)
	, SprintSpeed(550.0f)
	, bIsSprinting(false)
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
	SkillManager = CreateDefaultSubobject<USkillManager>(TEXT("SkillManager"));
	SoundComponent = CreateDefaultSubobject<USoundComponent>(TEXT("SoundComponent"));
	//Interaction
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Box"));
	InteractionBox->SetupAttachment(RootComponent);

	//Movement
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//ComboAttack
	MaxCombo = 5;
	AttackEndComboState();

	// Aiming
	AimingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimingTimeline"));
	DefaultCameraBoomSocketOffset = FVector(0.0f, 0.0f, 50.0f);
	RangeWeaponCameraBoomSocketOffset = FVector(0.0f, 60.0f, 50.0f);
	DefaultFOV = 90.0f;
	AimingFOV = 50.0f;
	bIsAiming = false;

	FatalFallHeight = 850.0f;
	LastGroundedZ = 0.0f;
	bShouldCheckFallDamage = false;
}


void ASinkDownProjectCharacter::BeginPlay()
{ 
	Super::BeginPlay();

	if (CombatComponent)
	{
		CombatComponent->SetLocationDelegate(FGetLocationDelegate::CreateUObject(this, &ASinkDownProjectCharacter::GetAttackOrigin));
		CombatComponent->SetForwardVectorDelegate(FGetForwardVectorDelegate::CreateUObject(this, &ASinkDownProjectCharacter::GetAttackDirection));
	}

	GetWorldTimerManager().SetTimer(InteractionCheckTimerHandle, this, &ASinkDownProjectCharacter::CheckForInteractables, InteractionCheckInterval, true);

	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		AnimInstance = CharacterMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &ASinkDownProjectCharacter::OnAttackMontageEnded);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASinkDownProjectCharacter::OnHandleMeleeAttackNotify);
		}
	}

	//PlayerLife
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ASinkDownProjectCharacter::OnPlayerDeath);
	}

	// Aiming
	InitializeAimingTimeline();
	if (FollowCamera)
	{
		DefaultFOV = FollowCamera->FieldOfView;
	}

	// UI Crosshair
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		CrosshAir = CreateWidget<UCrosshairWidget>(PlayerController, HUDClass);
		OnAimingProgressChanged.BindUObject(CrosshAir, &UCrosshairWidget::UpdateCrosshairSpread);
	}

	// Create a Diary Collection widget
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DiaryCollectionWidget = CreateWidget<UDiaryCollectionWidget>(PC, DiaryCollectionWidgetClass);
	}
}

void ASinkDownProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASinkDownProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASinkDownProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASinkDownProjectCharacter::Look);

		// Zoom In/Out
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ASinkDownProjectCharacter::HandleCameraZoom);

		// Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::OnInteract);

		// UnequipWeaponAction
		EnhancedInputComponent->BindAction(UnequipWeaponAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::UnequipWeapon);

		// SwitchWeaponAction
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::SwitchWeapon);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::Attack);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASinkDownProjectCharacter::StopSprint);

		// Aiming
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::StartAiming);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &ASinkDownProjectCharacter::StopAiming);

		// Skill(Gliding, Parry)
		EnhancedInputComponent->BindAction(GlidingAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::ActivateGlidingSkill);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::ActivateParrySkill);

		// Added Mugunghwa::WASD Minigame Input Bindings
		EnhancedInputComponent->BindAction(PressWAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::HandleWKeyPressed);
		EnhancedInputComponent->BindAction(PressAAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::HandleAKeyPressed);
		EnhancedInputComponent->BindAction(PressSAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::HandleSKeyPressed);
		EnhancedInputComponent->BindAction(PressDAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::HandleDKeyPressed);

		// DiaryWidget
		EnhancedInputComponent->BindAction(DiaryCollectionWidgetAction, ETriggerEvent::Started, this, &ASinkDownProjectCharacter::ToggleDiaryCollection);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASinkDownProjectCharacter::Move(const FInputActionValue& Value)
{
	if (MugunghwaGameComponent && (MugunghwaGameComponent->GetCurrentState() == EMugunghwaState::WASDGame || MugunghwaGameComponent->GetCurrentState() == EMugunghwaState::Failed)) return;
	if (bIsAttacking || SkillManager->IsSkillActive(ESkillType::Parry)) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASinkDownProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASinkDownProjectCharacter::HandleCameraZoom(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() != 0.0f)
	{
		float CurrentDistance = CameraBoom->TargetArmLength;

		float ZoomSpeed = 40.0f;
		float MinDistance = 80.0f;
		float MaxDistance = 600.0f;

		float NewDistance = CurrentDistance - (Value.Get<float>() * ZoomSpeed);
		NewDistance = FMath::Clamp(NewDistance, MinDistance, MaxDistance);

		CameraBoom->TargetArmLength = NewDistance;
	}
}

FVector ASinkDownProjectCharacter::GetAttackOrigin() const
{
	return WeaponManager->GetCurrentWeaponLocation();
}

FVector ASinkDownProjectCharacter::GetAttackDirection() const
{

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return FollowCamera->GetForwardVector();
	}

	int32 ViewportSizeX, ViewportSizeY;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ViewportCenter(ViewportSizeX / 2.0f + 50.0f, ViewportSizeY / 2.0f + -30.0f);

	FVector WorldLocation, WorldDirection;
	if (PC->DeprojectScreenPositionToWorld(ViewportCenter.X, ViewportCenter.Y, WorldLocation, WorldDirection))
	{
		FVector EndLocation = WorldLocation + WorldDirection * 10000.0f;

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, EndLocation, ECC_Pawn, QueryParams);

		if (bHit && HitResult.GetActor())
		{
			return (HitResult.Location - GetActorLocation()).GetSafeNormal();
		}

		return WorldDirection;
	}

	return FollowCamera->GetForwardVector();
}


// Interaction------------------------------------------------------
/**
 * Function called when the interaction button is pressed.
 * If an interface is currently set, it executes the interaction with the corresponding object.
 */
void ASinkDownProjectCharacter::OnInteract()
{
	if (Interface)
	{
		Interface->InteractionWithMe();
	}
}

/**
 * Function called periodically to check for interactable objects.
 * It finds the closest interactable actor and updates the interface.
 */
void ASinkDownProjectCharacter::CheckForInteractables()
{
	AActor* ClosestInteractableActor = FindClosestInteractableActor();
	/*if (ClosestInteractableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found closest interactable: %s"), *ClosestInteractableActor->GetName());
	}*/
	UpdateInteractionInterface(ClosestInteractableActor);
}

/**
 * Finds the closest interactable actor around the character.
 * @return The closest interactable actor, or nullptr if none found
 */
AActor* ASinkDownProjectCharacter::FindClosestInteractableActor()
{
	TArray<AActor*> OverlappingActors;
	InteractionBox->GetOverlappingActors(OverlappingActors);

	AActor* ClosestActor = nullptr;
	float ClosestDistance = FLT_MAX;

	for (AActor* CurrentActor : OverlappingActors)
	{
		if (CurrentActor->Implements<UInteractionInterface>())
		{
			float Distance = GetDistanceTo(CurrentActor);
			if (Distance < ClosestDistance)
			{
				ClosestActor = CurrentActor;
				ClosestDistance = Distance;
			}
		}
	}

	return ClosestActor;
}

/**
 * Updates the current interaction interface.
 * If the closest actor has changed, it replaces the interface.
 * @param NewClosestActor The newly found closest interactable actor
 */
void ASinkDownProjectCharacter::UpdateInteractionInterface(AActor* NewClosestActor)
{
	IInteractionInterface* NewInterface = Cast<IInteractionInterface>(NewClosestActor);

	if (Interface != NewInterface)
	{
		if (Interface)
		{
			Interface->HideInteractionWidget();
		}

		Interface = NewInterface;

		if (Interface)
		{
			Interface->ShowInteractionWidget();
		}
	}
}

/**
 * Function called when the character's lifecycle ends.
 * It cleans up any resources in use.
 * @param EndPlayReason The reason why the actor is being ended
 */
void ASinkDownProjectCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(InteractionCheckTimerHandle);
}


// AnimationMovement
void ASinkDownProjectCharacter::StartSprint()
{
	if (bIsAiming) return;
	if (MugunghwaGameComponent && (MugunghwaGameComponent->GetCurrentState() != EMugunghwaState::Inactive)) return;

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	// Start repeating movement sound
	GetWorldTimerManager().SetTimer(
		MovementSoundTimer,
		this,
		&ASinkDownProjectCharacter::PlayMovementSound,
		MovementSoundInterval,
		true 
	);
}

void ASinkDownProjectCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Clear the timer when stopping sprint
	GetWorldTimerManager().ClearTimer(MovementSoundTimer);
}

void ASinkDownProjectCharacter::PlayMovementSound()
{
	if (GetCharacterMovement()->IsMovingOnGround() && GetVelocity().SizeSquared() > 0)
	{
		if (SoundComponent)
		{
			SoundComponent->PlayWalkSound();
		}
	}
}


void ASinkDownProjectCharacter::UnequipWeapon()
{
	if (bIsAiming) return;
	if (SkillManager->IsAnySkillActive()) return;

	WeaponManager->DeactivateCurrentWeapon();
	CurrentWeapon = WeaponManager->GetCurrentWeapon();

	// Remove the CrosshAir
	CrosshAir->RemoveFromParent();

	// Adjust the camera view
	CameraBoom->SocketOffset = DefaultCameraBoomSocketOffset;
	FollowCamera->SetRelativeRotation(FRotator(0, 0, 0));

}

void ASinkDownProjectCharacter::SwitchWeapon()
{
	if (bIsAiming) return;
	if (SkillManager->IsAnySkillActive()) return;

	WeaponManager->SwitchWeapon();

	CurrentWeapon = WeaponManager->GetCurrentWeapon();
	if (CurrentWeapon)
	{
		EAttackType AttackType = (CurrentWeapon->WeaponType == EWeaponType::MeleeWeapon) ? EAttackType::AT_Melee : EAttackType::AT_Range;

		CombatComponent->SetAttackType(AttackType);
		CombatComponent->SetWeaponRange(CurrentWeapon->WeaponRange);
	}

	if (IsCurrentWeaponRanged())
	{
		CrosshAir->AddToViewport();

		CameraBoom->SocketOffset = RangeWeaponCameraBoomSocketOffset;
		FollowCamera->SetRelativeRotation(FRotator(0, 0, -5.0f));
	}
	else
	{
		CrosshAir->RemoveFromParent();

		CameraBoom->SocketOffset = DefaultCameraBoomSocketOffset;
		FollowCamera->SetRelativeRotation(FRotator(0, 0, 0));
	}
}

// Function used by PlayerAnimInstance to change the idle motion when acquiring a weapon.
bool ASinkDownProjectCharacter::GetActiveWeaponType(EWeaponType WeaponTypeToCheck) const
{
	return WeaponManager->GetActiveWeaponType() == WeaponTypeToCheck;
}

void ASinkDownProjectCharacter::Attack()
{

	if (!CurrentWeapon || !CombatComponent) return;

	if (SkillManager->IsAnySkillActive()) return;

	if (CurrentWeapon->WeaponType == EWeaponType::RangeWeapon)
	{
		// Call RangeAttack on CombatComponent
		CombatComponent->Attack(nullptr);  // Pass nullptr as we're not targeting a specific actor
	}
	else // Melee weapon
	{
		MeleeAttack();
	}
}
bool ASinkDownProjectCharacter::IsCurrentWeaponRanged() const
{
	return CurrentWeapon && CurrentWeapon->WeaponType == EWeaponType::RangeWeapon;
}

// Melee Attack Animation Related Functions
void ASinkDownProjectCharacter::OnHandleMeleeAttackNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("AttackHitCheck"))
	{
		if (CombatComponent)
		{
			CombatComponent->Attack(nullptr);
		}
	}

	if (NotifyName == FName("NextAttackCheck"))
	{
		bCanNextCombo = false;

		if (bIsComboInputOn)
		{
			AttackStartComboState();
			AnimInstance->Montage_JumpToSection(GetAttackMontageSectionName(CurrentCombo), MeleeAttackMontage);
		}
	}
}

void ASinkDownProjectCharacter::MeleeAttack()
{
	if (bIsAttacking)
	{
		if (bCanNextCombo)
		{
			bIsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		AnimInstance->Montage_Play(MeleeAttackMontage, 1.4f);
		AnimInstance->Montage_JumpToSection(GetAttackMontageSectionName(CurrentCombo), MeleeAttackMontage);
		bIsAttacking = true;
	}
}

void ASinkDownProjectCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	AttackEndComboState();
}

void ASinkDownProjectCharacter::AttackStartComboState()
{
	bCanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ASinkDownProjectCharacter::AttackEndComboState()
{
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0;
}

FName ASinkDownProjectCharacter::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack_%d"), Section));
}


void ASinkDownProjectCharacter::InitializeAimingTimeline()
{
	if (AimingCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("AimingTimelineProgress"));
		AimingTimeline->AddInterpFloat(AimingCurve, ProgressFunction);
	}
}
void ASinkDownProjectCharacter::AimingTimelineProgress(float Value)
{
	if (FollowCamera)
	{
		float NewFOV = FMath::Lerp(DefaultFOV, AimingFOV, Value);
		FollowCamera->SetFieldOfView(NewFOV);
	}

	OnAimingProgressChanged.ExecuteIfBound(Value);

	if (Value >= 1.0f && bIsAiming)
	{
		AimingTimeline->Stop();
	}
}

void ASinkDownProjectCharacter::StartAiming()
{
	if (!bIsAiming && IsCurrentWeaponRanged())
	{
		bIsAiming = true;
		AimingTimeline->PlayFromStart();
		CameraBoom->SocketOffset = FVector(300.0f, 60.0f, 50.0f);
	}
}

void ASinkDownProjectCharacter::StopAiming()
{
	if (bIsAiming)
	{
		bIsAiming = false;
		AimingTimeline->Reverse();
		CameraBoom->SocketOffset = RangeWeaponCameraBoomSocketOffset;
	}
}

void ASinkDownProjectCharacter::ActivateGlidingSkill()
{
	if (GetActiveWeaponType(EWeaponType::MeleeWeapon))
	{
		SkillManager->ActivateSkill(0);
	}
}

void ASinkDownProjectCharacter::ActivateParrySkill()
{	
	if (GetActiveWeaponType(EWeaponType::MeleeWeapon)) SkillManager->ActivateSkill(1);
}

// Player Death
void ASinkDownProjectCharacter::OnPlayerDeath()
{
	//  Disable player input
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	if (ASinkDownProjectGameMode* GameMode = Cast<ASinkDownProjectGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->RespawnPlayer(this);
	}
}

// Mugunghwa Game UI
void ASinkDownProjectCharacter::HandleWKeyPressed()
{
	if (MugunghwaGameComponent) MugunghwaGameComponent->ValidateKeyInput(EKeys::W);
}
void ASinkDownProjectCharacter::HandleAKeyPressed()
{
	if (MugunghwaGameComponent) MugunghwaGameComponent->ValidateKeyInput(EKeys::A);
}
void ASinkDownProjectCharacter::HandleSKeyPressed()
{
	if (MugunghwaGameComponent) MugunghwaGameComponent->ValidateKeyInput(EKeys::S);
}
void ASinkDownProjectCharacter::HandleDKeyPressed()
{
	if (MugunghwaGameComponent) MugunghwaGameComponent->ValidateKeyInput(EKeys::D);
}

// Related to Player Falling
void ASinkDownProjectCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (bShouldCheckFallDamage)
	{
		bool bIsGliding = SkillManager && SkillManager->IsSkillActive(ESkillType::Gliding);
		if (!bIsGliding)
		{
			float FallDistance = LastGroundedZ - GetActorLocation().Z;
			if (FallDistance >= FatalFallHeight)
			{
				if (HealthComponent)
				{
					HealthComponent->TakeMaxDamage();
				}
			}
		}
		bShouldCheckFallDamage = false;
	}
}

void ASinkDownProjectCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		LastGroundedZ = GetActorLocation().Z;
		bShouldCheckFallDamage = true;
	}
}


// DiaryWidget
void ASinkDownProjectCharacter::ToggleDiaryCollection()
{
	UE_LOG(LogTemp, Warning, TEXT("void ASinkDownProjectCharacter::ToggleDiaryCollection()"));
	if (DiaryCollectionWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found DiaryCollectionWidget"));

		DiaryCollectionWidget->ToggleVisibility();

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (DiaryCollectionWidget->IsVisible())
			{
				// accept game+UI input when the UI is open
				FInputModeGameAndUI InputMode;
				PC->SetInputMode(InputMode);
			}
			else
			{
				// When the UI is closed, use the game-only input
				FInputModeGameOnly InputMode;
				PC->SetInputMode(InputMode);
			}
		}
	}
}
