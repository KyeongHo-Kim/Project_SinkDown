#include "SinkDownProject/HealthSystem/MeleeEnemyDamageHandler.h"

UMeleeEnemyDamageHandler::UMeleeEnemyDamageHandler()
	: MaxHealth(0)
	, CurrentHealth(0)
	, bIsDead(false)
{
}

void UMeleeEnemyDamageHandler::Initialize(int32 InitialHealth)
{
	MaxHealth = InitialHealth;
	CurrentHealth = MaxHealth;
}

// applies damage to the enemy and updates their health status based on melee or ranged attacks.
void UMeleeEnemyDamageHandler::TakeDamage(bool bIsMelee)
{
	if (bIsDead) return;

	int DamageAmount;

	if (bIsMelee)
	{
		DamageAmount = FMath::RandRange(2, 4);
	}
	else
	{
		DamageAmount = FMath::RandRange(1, 2);
	}

	CurrentHealth = FMath::Max(0, CurrentHealth - DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("MeleeEnemyCurrentHealth: %d"), CurrentHealth);
	if (CurrentHealth <= 0)
	{
		bIsDead = true;
	}
}

bool UMeleeEnemyDamageHandler::IsDead() const
{
	return bIsDead;
}
