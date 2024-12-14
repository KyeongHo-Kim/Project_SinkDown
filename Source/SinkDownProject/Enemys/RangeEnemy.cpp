#include "SinkDownProject/Enemys/RangeEnemy.h"
#include "SinkDownProject/CombatSystem/CombatComponent.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"
#include "Animation/AnimInstance.h"

void ARangeEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (GetMesh() && GetMesh()->GetAnimInstance())
    {
        GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ARangeEnemy::OnNotifyFireProjectile);
    }

    if (CombatComponent)
    {
        CombatComponent->SetAttackType(EAttackType::AT_Range);
        if (EnemyData)
        {
            CombatComponent->SetWeaponRange(GetAttackRange());
        }
    }
}

void ARangeEnemy::Attack()
{
    PlayAttackMontage();
}

float ARangeEnemy::GetAttackRange()
{
    return Super::GetAttackRange();
}

float ARangeEnemy::GetDetectedRange()
{
    return Super::GetDetectedRange();
}

void ARangeEnemy::PlayAttackMontage()
{
    if (RangeAttackMontage)
    {
        PlayAnimMontage(RangeAttackMontage);
    }
}

float ARangeEnemy::PlayDeathMontage()
{
    if (RangeDeathMontage)
    {
        PlayAnimMontage(RangeDeathMontage);
        return RangeDeathMontage->GetPlayLength() - 0.3f;
    }
    return Super::PlayDeathMontage();
}

void ARangeEnemy::OnNotifyFireProjectile(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName == FName("FireProjectile"))
    {
        FireProjectile();
    }
}

void ARangeEnemy::OnAnimNotify(FName NotifyName)
{
    if (NotifyName == "FireProjectile")
    {
        FireProjectile();
    }
}

void ARangeEnemy::FireProjectile()
{
    if (CombatComponent)
    {
        CombatComponent->Attack(Player);
    }
}