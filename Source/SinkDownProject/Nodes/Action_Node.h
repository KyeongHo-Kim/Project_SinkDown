#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/Nodes/BehaviorTree_Node.h"
#include "Action_Node.generated.h"

DECLARE_DELEGATE_RetVal(ENodeStatus, FActionDelegate);

UCLASS()
class SINKDOWNPROJECT_API UAction_Node : public UBehaviorTree_Node
{
	GENERATED_BODY()

private:
	FActionDelegate Action;

public:
	virtual ENodeStatus Tick() override;
	void SetAction(FActionDelegate InAction) { Action = InAction; }
};
