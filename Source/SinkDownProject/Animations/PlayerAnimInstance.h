#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SinkDownProject/SkillSystem/SkillTypes.h"
#include "PlayerAnimInstance.generated.h"

class ASinkDownProjectCharacter;
class UHealthComponent;

UCLASS()
class SINKDOWNPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	TObjectPtr<ASinkDownProjectCharacter> Owner;
	TObjectPtr<UHealthComponent> HealthComponent;

	// --- Movement ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) float Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) bool bIsSprinting;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true"))bool bIsJumping;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true"))bool bJustLanded;

	//Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) bool bHasMeleeWeaponEquipped;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) bool bHasRangeWeaponEquipped;

	//Skills
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) bool bIsGliding;

	// --- Hit React ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) bool bIsHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) float HitReactDuration;
	FTimerHandle HitReactTimerHandle;
	
	// Death
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnimInstance", meta = (AllowPrivateAccess = "true")) bool bIsDead;
private:
	void UpdateMovementState();
	void UpdateAirState();
	void UpdateWeaponState();
	void UpdateSkillState();

	UFUNCTION() void OnTakeDamage();
	void ResetHitReact();
	void InitializeHealthComponent();

	UFUNCTION() void OnPlayerDeath();

public:
	void ResetDeathState();
};
