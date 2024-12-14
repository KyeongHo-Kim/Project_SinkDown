#include "SinkDownProject/Enemys/Enemy.h"
#include "SinkDownProject/DataAsset/EnemyData.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"
#include "SinkDownProject/CombatSystem/CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/Controller.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AEnemy::OnEnemyDeath);
	}

	if (CombatComponent)
	{
		CombatComponent->SetLocationDelegate(FGetLocationDelegate::CreateUObject(this, &AEnemy::GetAttackOrigin));
		CombatComponent->SetForwardVectorDelegate(FGetForwardVectorDelegate::CreateUObject(this, &AEnemy::GetActorForwardVector));
	}

	Player = Cast<ASinkDownProjectCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AEnemy::OnEnemyDeath()
{

	// Disable collisiona
	SetActorEnableCollision(false);

	// Getting AI controller references and stopping movement
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
	}

	PlayDeathMontage();

	AController* EnemyController = GetController();
	if (EnemyController)
	{
		EnemyController->UnPossess();
		EnemyController->Destroy();

		UE_LOG(LogTemp, Warning, TEXT("AEnemy::OnEnemyDeath()"));
	}

	float DeathMontageLength = PlayDeathMontage();
	SetLifeSpan(DeathMontageLength);
}

void AEnemy::Attack()
{
	PlayAttackMontage();
	CombatComponent->Attack(Player);
}

float AEnemy::GetAttackRange()
{
	return EnemyData->AttackRange;
}

float AEnemy::GetDetectedRange()
{
	return EnemyData->DetectedRange;
}

FVector AEnemy::GetAttackOrigin() const
{
	FVector AttackOrigin = GetActorLocation();
	return AttackOrigin + (Player->GetActorLocation() - AttackOrigin).GetSafeNormal();
}

