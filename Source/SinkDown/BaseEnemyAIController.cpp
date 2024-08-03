//#include "BaseEnemyAIController.h"
//#include "Nodes/Sequence_Node.h"
//#include "Nodes/Selector_Node.h"
//#include "Nodes/Action_Node.h"
//#include "Nodes/Condition_Node.h"
//#include "Kismet/GameplayStatics.h"
//#include "SinkDownCharacter.h"
//#include "CombatComponent.h"
//
//ABaseEnemyAIController::ABaseEnemyAIController() : 
//	bWasPlayerDetected(false), TimeSinceLastSawPlayer(FLT_MAX), SuspicionDuration(2.0f), ChaseDistance(600.0f)
//{
//	BuildBehaviorTree();
//}
//
//void ABaseEnemyAIController::BuildBehaviorTree()
//{
//	TUniquePtr<UCondition_Node> PlayerDetected = MakeUnique<UCondition_Node>([this]()
//	{ 
//		return IsPlayerDetected() && CombatComponent->CanAttack(Player);
//	});
//
//	TUniquePtr<UCondition_Node> PlayerLost = MakeUnique<UCondition_Node>([this]()
//	{
//		return !IsPlayerDetected(); // 플레이어가 탐지되지 않는 경우
//	});
//
//	TUniquePtr<UAction_Node> ChaseBehavior = MakeUnique<UAction_Node>([this]()
//	{
//		if (IsPlayerDetected()) // 플레이어가 여전히 탐지되고 있는지 확인
//		{
//			Chase();
//			return ENodeStatus::Running;
//		}
//		return ENodeStatus::Failure; // 플레이어가 탐지되지 않았다면 실패로 처리
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
//		MoveTemp(ChaseBehavior)
//	});
//
//	// 플레이어를 잃었을 때 SuspicionBehavior를 실행하는 시퀀스
//	TUniquePtr<USequence_Node> LostPlayerSequence = MakeUnique<USequence_Node>(TArray<TUniquePtr<IBehaviorTree_Node>>
//	{
//		MoveTemp(PlayerLost),
//		MoveTemp(SuspicionBehavior)
//	});
//	
//	TUniquePtr<USelector_Node> BehaviorSelector = MakeUnique<USelector_Node>(TArray<TUniquePtr<IBehaviorTree_Node>>
//	{
//		MoveTemp(AttackSequence),
//		MoveTemp(LostPlayerSequence), // 플레이어를 잃었을 때 의심 행동
//		MoveTemp(PatrolBehavior) // 기본 순찰 행동
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
//
//
//bool ABaseEnemyAIController::IsPlayerDetected()
//{
//	if (!Player)
//	{
//		return false;
//	}
//
//	FVector ActorLocation = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;
//	float DistanceToPlayer = FVector::Distance(ActorLocation, Player->GetActorLocation());
//
//	return DistanceToPlayer <= ChaseDistance;
//}
//
//void ABaseEnemyAIController::Chase()
//{
//}
//
//void ABaseEnemyAIController::Suspicion()
//{
//}
//
//void ABaseEnemyAIController::Patrol()
//{
//}
