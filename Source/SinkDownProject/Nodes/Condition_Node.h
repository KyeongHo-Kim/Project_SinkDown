#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/Nodes/BehaviorTree_Node.h"
#include "Condition_Node.generated.h"

DECLARE_DELEGATE_RetVal(bool, FConditionDelegate)

UCLASS()
class SINKDOWNPROJECT_API UCondition_Node : public UBehaviorTree_Node
{
	GENERATED_BODY()
	
private:
	FConditionDelegate Condition;

public:
	virtual ENodeStatus Tick() override;
	void SetCondition(FConditionDelegate InCondition) { Condition = InCondition; }
};
