#include "SinkDownProject/Enemys/MeleeEnemy.h"
#include "SinkDownProject/CombatSystem/CombatComponent.h"

void AMeleeEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (CombatComponent)
    {
        CombatComponent->SetAttackType(EAttackType::AT_Melee);
        if (EnemyData)
        {
            CombatComponent->SetWeaponRange(GetAttackRange());
        }
    }
}

void AMeleeEnemy::Attack()
{
    Super::Attack();
}

float AMeleeEnemy::GetAttackRange()
{
   return Super::GetAttackRange();
}

float AMeleeEnemy::GetDetectedRange()
{
    return Super::GetDetectedRange();
}

void AMeleeEnemy::PlayAttackMontage()
{
    if (MeleeAttackMontage)
    {
        PlayAnimMontage(MeleeAttackMontage);
    }
}

float AMeleeEnemy::PlayDeathMontage()
{
    if (MeleeDeathMontage)
    {
        PlayAnimMontage(MeleeDeathMontage);
        return MeleeDeathMontage->GetPlayLength() - 0.3f;
    }
    return Super::PlayDeathMontage();
}


