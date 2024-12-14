#include "SinkDownProject/Animations/EnemyBaseAnimInstance.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "SinkDownProject/Enemys/MeleeEnemyAIController.h"
#include "SinkDownProject/Enemys/RangeEnemyAIController.h"

UEnemyBaseAnimInstance::UEnemyBaseAnimInstance()
	: bIsEnemyTypeInitialized(false)
	, Speed(0.0f)
	, bIsMoving(false)
	, Character(nullptr)
{
}

void UEnemyBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UEnemyBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character)
	{
		Character = Cast<ACharacter>(TryGetPawnOwner());
		return;
	}

	if (!Character->GetController())
	{
		return;
	}
	if (!bIsEnemyTypeInitialized)
	{
		InitializeEnemyType();
	}


	UpdateMovementState();
	UpdateCombatState();
}

void UEnemyBaseAnimInstance::InitializeEnemyType()
{
	if (AMeleeEnemyAIController* MeleeEnemyController = Cast<AMeleeEnemyAIController>(Character->GetController()))
	{
		CachedMeleeController = MeleeEnemyController;
		CurrentEnemyType = EEnemyType::MeleeEnemy;
	}
	else if (ARangeEnemyAIController* RangeEnemyController = Cast<ARangeEnemyAIController>(Character->GetController()))
	{
		CachedRangeController = RangeEnemyController;
		CurrentEnemyType = EEnemyType::RangeEnemy;
	}

	bIsEnemyTypeInitialized = true;
}

void UEnemyBaseAnimInstance::UpdateMovementState()
{
	FVector Velocity = Character->GetVelocity();
	Speed = Velocity.Size();
	bIsMoving = Speed > 1.0f;
}

void UEnemyBaseAnimInstance::UpdateCombatState()
{
	switch (CurrentEnemyType)
	{
	case EEnemyType::MeleeEnemy:
		bIsDetected = CachedMeleeController->IsDetectingPlayer();
		break;
	case EEnemyType::RangeEnemy:
		bIsDetected = CachedRangeController->IsDetectingPlayer();
		break;
	case EEnemyType::BossEnemy:
		break;
	default:
		break;
	}
}

