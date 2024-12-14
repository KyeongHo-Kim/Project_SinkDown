#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAIController.h"
#include "RangeEnemyAIController.generated.h"

class ARangeEnemy;

UCLASS()
class SINKDOWNPROJECT_API ARangeEnemyAIController : public ABaseEnemyAIController
{
    GENERATED_BODY()

public:
    ARangeEnemyAIController();

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void InitializeBehaviorTree() override;
    virtual bool IsPlayerDetected(float DetectedRange) override;
    virtual bool IsInAttackRange(float AttackRange) override;
    virtual ENodeStatus Suspicion() override;

    UPROPERTY() ARangeEnemy* RangeEnemy;

    bool bCanAttack;
    float AttackCooldown;
    FTimerHandle AttackTimerHandle;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    ENodeStatus RangeAttack();

    void ResetAttackCooldown();

public:
    bool IsDetectingPlayer() const { return bIsPlayerDetected; }
};