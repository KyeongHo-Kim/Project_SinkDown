#include "BaseEnemyAIController.h"
#include "Nodes/Sequence_Node.h"
#include "Nodes/Selector_Node.h"
#include "Nodes/Action_Node.h"
#include "Nodes/Condition_Node.h"


ABaseEnemyAIController::ABaseEnemyAIController() 
{
	InitializeBehaviorTree();
}



void ABaseEnemyAIController::InitializeBehaviorTree()
{

	Test_Action = CreateDefaultSubobject<UAction_Node>(TEXT("TestAction"));
	Test_Action->SetAction(FActionDelegate::CreateUObject(this, &ABaseEnemyAIController::TEST));

	RootNode = CreateDefaultSubobject<USequence_Node>(TEXT("RootNode"));
	RootNode->AddChildNode(Test_Action);

}

ENodeStatus ABaseEnemyAIController::TEST()
{
	UE_LOG(LogTemp, Warning, TEXT("TEST Function"));
	return ENodeStatus::Success;
}



void ABaseEnemyAIController::BeginPlay()
{
}

void ABaseEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UE_LOG(LogTemp, Warning, TEXT("TEST Function false"));
	if (RootNode.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("TEST Function"));
		RootNode->Tick();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("TEST Function false"));
}