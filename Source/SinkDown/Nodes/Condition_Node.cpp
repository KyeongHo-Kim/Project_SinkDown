#include "Condition_Node.h"

ENodeStatus UCondition_Node::Tick()
{
	//return Condition.Execute() ? ENodeStatus::Success : ENodeStatus::Failure;

    bool bConditionMet = Condition.Execute();
    UE_LOG(LogTemp, Log, TEXT("Condition Node Tick: %s"), bConditionMet ? TEXT("Success") : TEXT("Failure"));

    return bConditionMet ? ENodeStatus::Success : ENodeStatus::Failure;
}
