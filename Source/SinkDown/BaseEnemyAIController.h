//#pragma once
//
//#include "CoreMinimal.h"
//#include "AIController.h"
//#include "Nodes/BehaviorTree_Node.h"
//#include "BaseEnemyAIController.generated.h"
//
//class ASinkDownCharacter;
//class UCombatComponent;
//
//UCLASS()
//class SINKDOWN_API ABaseEnemyAIController : public AAIController
//{
//	GENERATED_BODY()
//
//public:
//	ABaseEnemyAIController();
//	virtual void Tick(float DeltaSeconds) override;
//
//protected:
//	virtual void BeginPlay() override;
//
//	TUniquePtr<IBehaviorTree_Node> BehaviorTreeRoot;
//	virtual void BuildBehaviorTree();
//	
//	virtual void Attack() = 0;
//	virtual bool IsPlayerDetected();
//	virtual void Chase();
//	virtual void Suspicion();
//	virtual void Patrol(); 
//
//	bool bWasPlayerDetected;
//	float TimeSinceLastSawPlayer;
//	UPROPERTY(EditAnywhere, Category = "Time") float SuspicionDuration;
//	UPROPERTY(EditAnywhere, Category = "Distance") float ChaseDistance;
//
//	//Patrol
//	float TimeSinceArrivedPath;
//	float WaypointRange = 1.0f;
//	float WaypointDelay = 1.0f;
//	int32 CurrentWaypointIndex = 0;
//
//private:
//	UPROPERTY(VisibleAnywhere, Category = "Player") ASinkDownCharacter* Player;
//	UPROPERTY(VisibleAnywhere, Category = "CombatComponent") UCombatComponent* CombatComponent;
//
//};
