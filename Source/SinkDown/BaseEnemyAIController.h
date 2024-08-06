#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Nodes/BehaviorTree_Node.h"
#include "BaseEnemyAIController.generated.h"

class UAction_Node;
class UCondition_Node;
class USequence_Node;
class USelector_Node;

class ASinkDownCharacter;
class UCombatComponent;

UCLASS()
class SINKDOWN_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseEnemyAIController();
	virtual void Tick(float DeltaSeconds) override;

private:


protected:
	virtual void BeginPlay() override;

	void InitializeBehaviorTree();

	TWeakObjectPtr<USequence_Node> RootNode;
	TWeakObjectPtr<UCondition_Node> PlayerDetected_Condition;
	TWeakObjectPtr<UAction_Node> Test_Action;



	ENodeStatus TEST();
};
