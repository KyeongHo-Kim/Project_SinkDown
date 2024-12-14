#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAIController.h"
#include "MeleeEnemyAIController.generated.h"

class AMeleeEnemy;

UCLASS()
class SINKDOWNPROJECT_API AMeleeEnemyAIController : public ABaseEnemyAIController
{
	GENERATED_BODY()

public:
	AMeleeEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void InitializeBehaviorTree() override;

	virtual bool IsPlayerDetected(float Range) override;
	virtual bool IsInAttackRange(float AttackRange) override;

	UPROPERTY() AMeleeEnemy* MeleeEnemy;

	FTimerHandle AttackTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float AttackCooldown;
	bool bCanAttack;

	UFUNCTION(BlueprintCallable, Category = "Combat") ENodeStatus MeleeAttack();
	UFUNCTION() void ResetAttackCooldown();

public:
	bool IsDetectingPlayer() const { return bIsPlayerDetected; }
};
