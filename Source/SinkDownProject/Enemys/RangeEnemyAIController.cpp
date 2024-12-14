#include "SinkDownProject/Enemys/RangeEnemyAIController.h"
#include "SinkDownProject/Enemys/RangeEnemy.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

ARangeEnemyAIController::ARangeEnemyAIController()
    : bCanAttack(true)
    , AttackCooldown(3.0f)
{
}

void ARangeEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    RangeEnemy = Cast<ARangeEnemy>(InPawn);
}

void ARangeEnemyAIController::InitializeBehaviorTree()
{
    Super::InitializeBehaviorTree();

    UAction_Node* RangeAttack_Action = NewObject<UAction_Node>(this);
    RangeAttack_Action->SetAction(FActionDelegate::CreateUObject(this, &ARangeEnemyAIController::RangeAttack));

    USequence_Node* Combat_Sequence = Cast<USequence_Node>(RootNode->GetChildren()[0]);
    if (Combat_Sequence)
    {
        Combat_Sequence->AddChild(RangeAttack_Action);
    }
}

ENodeStatus ARangeEnemyAIController::RangeAttack()
{
    if (StrikeRaange(20.0f, RangeEnemy->GetAttackRange()) && bCanAttack)
    {
        bIsAttacking = true;
        RangeEnemy->Attack();

        bCanAttack = false;
        GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ARangeEnemyAIController::ResetAttackCooldown, AttackCooldown, false);

        return ENodeStatus::Success;
    }
    else if (!bCanAttack)
    {
        return ENodeStatus::Running;
    }
    else
    {
        return ENodeStatus::Failure;
    }
}

void ARangeEnemyAIController::ResetAttackCooldown()
{
    bCanAttack = true;
    bIsAttacking = false;
}

bool ARangeEnemyAIController::IsPlayerDetected(float DetectedRange)
{
    return Super::IsPlayerDetected(RangeEnemy->GetDetectedRange());
}

bool ARangeEnemyAIController::IsInAttackRange(float AttackRange)
{
    return Super::IsInAttackRange(RangeEnemy->GetAttackRange());
}

ENodeStatus ARangeEnemyAIController::Suspicion()
{
    return Super::Suspicion();
}