#include "SinkDownProject/Animations/PlayerAnimInstance.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SinkDownProject/DataAsset/WeaponData.h"
#include "Kismet/KismetMathLibrary.h"
#include "SinkDownProject/SkillSystem/SkillManager.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
	: Owner(nullptr)
    , bIsHit(false)
    , HitReactDuration(1.0f)
{

}

void UPlayerAnimInstance::NativeBeginPlay()
{
	ASinkDownProjectCharacter* Character = Cast<ASinkDownProjectCharacter>(TryGetPawnOwner());
	if (Character)
	{
		Owner = Character;
        InitializeHealthComponent();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!IsValid(Owner))
    {
        return;
    }

    if (bIsDead)
    {
        // Reset air state variables when dead
        bIsInAir = false;
        bIsJumping = false;
        bJustLanded = false;
        return;
    }

    UpdateMovementState();
    UpdateAirState();
    UpdateWeaponState();
    UpdateSkillState();
}

void UPlayerAnimInstance::UpdateMovementState()
{
    Speed = Owner->GetVelocity().Length();
    bIsSprinting = Owner->IsSprinting();

    // Calculate Direction
    FRotator RotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(
        Owner->GetActorRotation(),
        Owner->GetVelocity().Rotation()
    );
    Direction = -RotationDelta.Yaw;

    if (bHasMeleeWeaponEquipped || bHasRangeWeaponEquipped)
    {
        Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
    }
    else
    {
        Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
    }
}


void UPlayerAnimInstance::UpdateAirState()
{
    if (bIsDead) return;  // Don't update air state if dead

    bool bWasInAir = bIsInAir;
    bIsInAir = Owner->GetCharacterMovement()->IsFalling();

    // Jumping detection
    if (!bWasInAir && bIsInAir && Owner->GetVelocity().Z > 0)
    {
        bIsJumping = true;
    }
    else if (bIsJumping && Owner->GetVelocity().Z <= 0)
    {
        bIsJumping = false;
    }

    // Landing detection
    bJustLanded = bWasInAir && !bIsInAir;
}


void UPlayerAnimInstance::UpdateWeaponState()
{
    bHasMeleeWeaponEquipped = Owner->GetActiveWeaponType(EWeaponType::MeleeWeapon);
    bHasRangeWeaponEquipped = Owner->GetActiveWeaponType(EWeaponType::RangeWeapon);
}

void UPlayerAnimInstance::UpdateSkillState()
{
    if (Owner->GetSkillManager())
    {
        bIsGliding = Owner->GetSkillManager()->IsSkillActive(ESkillType::Gliding);
    }
}

void UPlayerAnimInstance::InitializeHealthComponent()
{
    if (Owner)
    {
        HealthComponent = Owner->FindComponentByClass<UHealthComponent>();
        if (HealthComponent)
        {
            // Dynamic delegate binding to handle damage events
            HealthComponent->OnTakeDamage.AddDynamic(this, &UPlayerAnimInstance::OnTakeDamage);

            HealthComponent->OnDeath.AddDynamic(this, &UPlayerAnimInstance::OnPlayerDeath);
        }
    }
}

void UPlayerAnimInstance::OnTakeDamage()
{
    bIsHit = true;

    if (Owner)
    {
        // Clear existing timer if any
        if (Owner->GetWorld()->GetTimerManager().IsTimerActive(HitReactTimerHandle))
        {
            Owner->GetWorld()->GetTimerManager().ClearTimer(HitReactTimerHandle);
        }

        // Set timer to reset hit react state
        Owner->GetWorld()->GetTimerManager().SetTimer(
            HitReactTimerHandle,
            this,
            &UPlayerAnimInstance::ResetHitReact,
            HitReactDuration,
            false
        );
    }
}

void UPlayerAnimInstance::ResetHitReact()
{
    bIsHit = false;
}


void UPlayerAnimInstance::OnPlayerDeath()
{
    /*bIsDead = true;*/
    bIsDead = true;

    // Reset movement and air states
    bIsInAir = false;
    bIsJumping = false;
    bJustLanded = false;
    Speed = 0.0f;
    Direction = 0.0f;
    bIsSprinting = false;

    // Reset combat states
    bIsHit = false;

    // Clear any active timers
    if (Owner && Owner->GetWorld())
    {
        Owner->GetWorld()->GetTimerManager().ClearTimer(HitReactTimerHandle);
    }
}

void UPlayerAnimInstance::ResetDeathState()
{
    bIsDead = false;
}
