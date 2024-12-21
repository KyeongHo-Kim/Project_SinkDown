#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SinkDownProject/CombatSystem/TeamInterface.h"
#include "SinkDownProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
// Combat
class UHealthComponent;
// Healath
class UCombatComponent;
// Interaction
class UBoxComponent;
class IInteractionInterface;
// Weapon
class UWeaponManager;
class UWeaponData;
// Aiming
class UTimelineComponent;
class UCurveFloat;
class UCrosshairWidget;
// Skill
class USkillManager;
// MugunghwaGame
class UMugunghwaGameComponent;
// Sound
class USoundComponent;
// Diary UI
class UDiaryCollectionWidget;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
// Crosshair Animation Delegate
DECLARE_DELEGATE_OneParam(FOnAimingProgressChanged, float);

UCLASS(config=Game)
class ASinkDownProjectCharacter : public ACharacter, public ITeamInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	/** Interaction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Switch Weapon Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UnequipWeaponAction;

	/** Switch Weapon Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchWeaponAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimingAction;

	/**Skill_Gliding Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GlidingAction;

	/**Skill_Parry Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ParryAction;

	/** WASD Mini Game Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PressWAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PressAAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PressSAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PressDAction;

	/**DiaryCollectionWidget Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DiaryCollectionWidgetAction;


public:
	ASinkDownProjectCharacter();
	virtual void Tick(float DeltaTime) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	//Zoom
	void HandleCameraZoom(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Landed(const FHitResult& Hit) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Combat") UCombatComponent* CombatComponent;
	UPROPERTY(VisibleAnywhere, Category = "Health") UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, Category = "WeaponManager") UWeaponManager* WeaponManager;
	UPROPERTY(VisibleAnywhere, Category = "SkillManager") USkillManager* SkillManager;
	UPROPERTY(VisibleAnywhere, Category = "Sound") USoundComponent* SoundComponent;

	//Interaction
	UPROPERTY(EditAnywhere, Category = "Interaction") UBoxComponent* InteractionBox; // Create volume around character when close to other objects checked
	IInteractionInterface* Interface;
	FTimerHandle InteractionCheckTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Interaction") float InteractionCheckInterval;

	void OnInteract();
	void CheckForInteractables();
	AActor* FindClosestInteractableActor();
	void UpdateInteractionInterface(AActor* NewClosestActor);


	// Weapon
	UWeaponData* CurrentWeapon;

	bool IsCurrentWeaponRanged() const;

	// Team
	UPROPERTY(EditAnywhere, Category = "Team") ETeam CurrentTeam = ETeam::Player;

	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true")) float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true")) float SprintSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) bool bIsSprinting;

	//(Melee)ComboAttack Variables
	int32 CurrentCombo;
	int32 MaxCombo;
	bool bCanNextCombo;
	bool bIsComboInputOn;
	bool bIsAttacking;


	// Aiming
	UPROPERTY(EditAnywhere, Category = "Camera") FVector DefaultCameraBoomSocketOffset;
	UPROPERTY(EditAnywhere, Category = "Camera") FVector RangeWeaponCameraBoomSocketOffset;
	float DefaultFOV;
	float AimingFOV;
	bool bIsAiming;
	UPROPERTY(VisibleAnywhere, Category = "Aiming") UTimelineComponent* AimingTimeline;
	UPROPERTY(EditAnywhere, Category = "Aiming") UCurveFloat* AimingCurve;
	
	UFUNCTION() void AimingTimelineProgress(float Value);
	void InitializeAimingTimeline();


	UPROPERTY(EditAnywhere, Category = "UI") TSubclassOf<UCrosshairWidget> HUDClass;
	UPROPERTY() UCrosshairWidget* CrosshAir;
	FOnAimingProgressChanged OnAimingProgressChanged;

	FVector GetAttackOrigin() const;
	FVector GetAttackDirection() const;

	UFUNCTION() void OnPlayerDeath();

	//MugunghwaGame
	UPROPERTY() UMugunghwaGameComponent* MugunghwaGameComponent;
	void HandleWKeyPressed();
	void HandleAKeyPressed();
	void HandleSKeyPressed();
	void HandleDKeyPressed();

	UPROPERTY(EditAnywhere, Category = "Fall Damage") float FatalFallHeight;
	float LastGroundedZ;
	bool bShouldCheckFallDamage;

public:

	UPROPERTY() UAnimInstance* AnimInstance;
	UPROPERTY(EditAnywhere, Category = "Animation") UAnimMontage* MeleeAttackMontage;
	
	virtual ETeam GetTeam() const override { return CurrentTeam; }
	virtual void SetTeam(ETeam NewTeam) override { CurrentTeam = NewTeam; }

	void StartSprint();
	void StopSprint();
	float MovementSoundInterval = 0.2f;
	FTimerHandle MovementSoundTimer;
	void PlayMovementSound();
	UFUNCTION(BlueprintCallable, Category = "Movement") bool IsSprinting() const { return bIsSprinting; }

	// -- Combat Function --
	void UnequipWeapon();
	void SwitchWeapon();
	void Attack();
	// -- Melee Attack Animation Related Functions --
	void MeleeAttack();
	void AttackStartComboState();
	void AttackEndComboState();
	FName GetAttackMontageSectionName(int32 Section);
	UFUNCTION() void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION() void OnHandleMeleeAttackNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION(BlueprintCallable, Category = "Weapon") bool GetActiveWeaponType(EWeaponType WeaponTypeToCheck) const;

	void StartAiming();
	void StopAiming();

	// -- Skill --
	void ActivateGlidingSkill();
	void ActivateParrySkill();
	FORCEINLINE USkillManager* GetSkillManager() const { return SkillManager; }

	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	//MugunghwaGame
	void SetMugunghwaGameComponent(UMugunghwaGameComponent* NewComponent) { MugunghwaGameComponent = NewComponent; }
	UMugunghwaGameComponent* GetMugunghwaGameComponent() const { return MugunghwaGameComponent; }

private:
	UPROPERTY() UDiaryCollectionWidget* DiaryCollectionWidget;
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UDiaryCollectionWidget> DiaryCollectionWidgetClass;

	void ToggleDiaryCollection();

};

