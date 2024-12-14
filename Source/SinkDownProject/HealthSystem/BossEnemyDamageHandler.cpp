#include "SinkDownProject/HealthSystem/BossEnemyDamageHandler.h"

UBossEnemyDamageHandler::UBossEnemyDamageHandler()
	: MaxHealth(0)
	, CurrentHealth(0)
	, bIsDead(false)
{
}

void UBossEnemyDamageHandler::Initialize(int32 InitialHealth)
{
	MaxHealth = InitialHealth;
	CurrentHealth = MaxHealth;
}

void UBossEnemyDamageHandler::TakeDamage(bool bIsMelee)
{
}

bool UBossEnemyDamageHandler::IsDead() const
{
	return bIsDead;
}
