#include "SinkDownProject/HealthSystem/PlayerDamageHandler.h"

UPlayerDamageHandler::UPlayerDamageHandler()
	: MaxHealth(0)
	, CurrentHealth(0)
	, bIsDead(false)
{
}

void UPlayerDamageHandler::Initialize(int32 InitialHealth)
{
	MaxHealth = InitialHealth;
	CurrentHealth = MaxHealth;
	bIsDead = false;
}

void UPlayerDamageHandler::TakeDamage(bool bIsMelee)
{
	if (bIsDead) return;

	int DamageAmount;

	if (bIsMelee)
	{
		DamageAmount = FMath::RandRange(10, 16);
	}
	else
	{
		DamageAmount = FMath::RandRange(8, 12);
	}

	CurrentHealth = FMath::Max(0, CurrentHealth - DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("CurrentHealth: %d"), CurrentHealth);
	if (CurrentHealth <= 0)
	{
		bIsDead = true;
	}
}

bool UPlayerDamageHandler::IsDead() const
{
	return bIsDead;
}
