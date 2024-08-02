#include "BaseEnemyAIController.h"
#include "Nodes/Sequence_Node.h"
#include "Nodes/Selector_Node.h"
#include "Nodes/Action_Node.h"
#include "Nodes/Condition_Node.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownCharacter.h"
#include "CombatComponent.h"

ABaseEnemyAIController::ABaseEnemyAIController() : 
	bWasPlayerDetected(false), TimeSinceLastSawPlayer(FLT_MAX), SuspicionDuration(2.0f), ChaseDistance(600.0f)
{
	//BuildBehaviorTree();
}
//
//void ABaseEnemyAIController::BuildBehaviorTree()
//{
//	TUniquePtr<UCondition_Node> PlayerDetected = MakeUnique<UCondition_Node>([this]()
//	{ 
//		return IsPlayerDetected();
//	});
//
//	TUniquePtr<UCondition_Node> CanAttackPlayer = MakeUnique<UCondition_Node>([this]()
//	{
//		return CombatComponent->CanAttack(Player);
//	});
//
//	TUniquePtr<UAction_Node> ChaseBehavior = MakeUnique<UAction_Node>([this]()
//	{
//		Chase();
//		return ENodeStatus::Running;
//	});
//
//	TUniquePtr<UAction_Node> SuspicionBehavior = MakeUnique<UAction_Node>([this]()
//	{
//		Suspicion();
//		return ENodeStatus::Running;
//	});
//
//	TUniquePtr<UAction_Node> PatrolBehavior = MakeUnique<UAction_Node>([this]()
//	{
//		Patrol();
//		return ENodeStatus::Running;
//	});
//
//	// 공격 시퀀스 (플레이어 탐지 -> 추적 -> 공격), 공격은 서브 클래스에서 추가 예정
//	TUniquePtr<USequence_Node> AttackSequence = MakeUnique<USequence_Node>(TArray<TUniquePtr<IBehaviorTree_Node>>
//	{
//		MoveTemp(PlayerDetected), 
//		MoveTemp(CanAttackPlayer),
//		MoveTemp(ChaseBehavior)
//	});
//
//	TUniquePtr<USelector_Node> BehaviorSelector = MakeUnique<USelector_Node>(TArray<TUniquePtr<IBehaviorTree_Node>>
//	{
//		MoveTemp(AttackSequence), 
//		MoveTemp(SuspicionBehavior), 
//		MoveTemp(PatrolBehavior)
//	});
//
//	BehaviorTreeRoot = MoveTemp(BehaviorSelector);
//}
//
//void ABaseEnemyAIController::BeginPlay()
//{
//	Super::BeginPlay();
//
//	Player = Cast<ASinkDownCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
//}
