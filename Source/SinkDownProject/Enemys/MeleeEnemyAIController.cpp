#include "SinkDownProject/Enemys/MeleeEnemyAIController.h"
#include "SinkDownProject/Enemys/MeleeEnemy.h"

AMeleeEnemyAIController::AMeleeEnemyAIController() 
    : AttackCooldown(4.0f)
    , bCanAttack(true)
{
}

void AMeleeEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    MeleeEnemy = Cast<AMeleeEnemy>(InPawn);
}

void AMeleeEnemyAIController::InitializeBehaviorTree()
{
    Super::InitializeBehaviorTree();

    UAction_Node* MeleeAttack_Action = NewObject<UAction_Node>(this);
    MeleeAttack_Action->SetAction(FActionDelegate::CreateUObject(this, &AMeleeEnemyAIController::MeleeAttack));

    USequence_Node* Combat_Sequence = Cast<USequence_Node>(RootNode->GetChildren()[0]);
    if (Combat_Sequence)
    {
        Combat_Sequence->AddChild(MeleeAttack_Action);
    }
}


ENodeStatus AMeleeEnemyAIController::MeleeAttack()
{
    if (StrikeRaange(20.0f, MeleeEnemy->GetAttackRange()) && bCanAttack)
    {
        bIsAttacking = true;
        MeleeEnemy->Attack();

        bCanAttack = false;
        GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AMeleeEnemyAIController::ResetAttackCooldown, AttackCooldown, false);

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

void AMeleeEnemyAIController::ResetAttackCooldown()
{
    bCanAttack = true;
    bIsAttacking = false;
}

bool AMeleeEnemyAIController::IsPlayerDetected(float Range)
{
    return Super::IsPlayerDetected(MeleeEnemy->GetDetectedRange());
}


bool AMeleeEnemyAIController::IsInAttackRange(float AttackRange)
{
    return Super::IsInAttackRange(MeleeEnemy->GetAttackRange());
}

