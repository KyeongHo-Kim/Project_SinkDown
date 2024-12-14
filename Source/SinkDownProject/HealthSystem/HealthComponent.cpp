#include "SinkDownProject/HealthSystem/HealthComponent.h"
#include "SinkDownProject/HealthSystem/PlayerDamageHandler.h"
#include "SinkDownProject/HealthSystem/BossEnemyDamageHandler.h"
#include "SinkDownProject/HealthSystem/MeleeEnemyDamageHandler.h"
#include "SinkDownProject/HealthSystem/RangeEnemyDamageHandler.h"
#include "SinkDownProject/DataAsset/EnemyData.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyData)
	{
		MaxHealth = EnemyData->Health;

		switch (EnemyData->EnemyType)
		{
		case EEnemyType::Player:
			DamageHandler = NewObject<UPlayerDamageHandler>(this);
			Cast<UPlayerDamageHandler>(DamageHandler)->Initialize(MaxHealth);
			break;
		case EEnemyType::BossEnemy:
			DamageHandler = NewObject<UBossEnemyDamageHandler>(this);
			Cast<UBossEnemyDamageHandler>(DamageHandler)->Initialize(MaxHealth);
			break;
		case EEnemyType::MeleeEnemy:
			DamageHandler = NewObject<UMeleeEnemyDamageHandler>(this);
			Cast<UMeleeEnemyDamageHandler>(DamageHandler)->Initialize(MaxHealth);
			break;
		case EEnemyType::RangeEnemy:
			DamageHandler = NewObject<URangeEnemyDamageHandler>(this);
			Cast<URangeEnemyDamageHandler>(DamageHandler)->Initialize(MaxHealth); 
			break;
		default:
			break;
		}
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UHealthComponent::IsDead() const
{
	return Cast<IDamageHandlerInterface>(DamageHandler)->IsDead();
}

void UHealthComponent::TakeDamage(bool bIsMelee)
{
	if (DamageHandler)
	{
		IDamageHandlerInterface* DamageHandlerInterface = Cast<IDamageHandlerInterface>(DamageHandler);
		if (DamageHandlerInterface)
		{
			DamageHandlerInterface->TakeDamage(bIsMelee);

			// Broadcast damage event without parameter
			OnTakeDamage.Broadcast();

			// Death status checks and events
			if (DamageHandlerInterface->IsDead())
			{
				OnDeath.Broadcast();
			}
		}
	}
}

void UHealthComponent::TakeMaxDamage()
{
	if (DamageHandler)
	{
		OnTakeDamage.Broadcast();
		OnDeath.Broadcast();
	}
}

void UHealthComponent::Reset()
{
	if (DamageHandler)
	{
		IDamageHandlerInterface* DamageHandlerInterface = Cast<IDamageHandlerInterface>(DamageHandler);
		if (DamageHandlerInterface)
		{
			DamageHandlerInterface->Initialize(MaxHealth);
		}
	}
}
