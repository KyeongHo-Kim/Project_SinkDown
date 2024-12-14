#include "SinkDownProject/HealthSystem/RangeEnemyDamageHandler.h"

URangeEnemyDamageHandler::URangeEnemyDamageHandler()
	: MaxHealth(0)
	, CurrentHealth(0)
	, bIsDead(false)
{
}


void URangeEnemyDamageHandler::Initialize(int32 InitialHealth)
{
	MaxHealth = InitialHealth;
	CurrentHealth = MaxHealth;
}

// applies damage to the enemy and updates their health status based on melee or ranged attacks.
void URangeEnemyDamageHandler::TakeDamage(bool bIsMelee)
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
	UE_LOG(LogTemp, Warning, TEXT("RangeEnemyCurrentHealth: %d"), CurrentHealth);
	if (CurrentHealth <= 0)
	{
		bIsDead = true;
	}
}

bool URangeEnemyDamageHandler::IsDead() const
{
	return bIsDead;
}
