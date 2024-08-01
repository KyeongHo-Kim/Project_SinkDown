#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BehaviorTree_Node.h"
#include "Action_Node.generated.h"

DECLARE_DELEGATE_RetVal(ENodeStatus, FActionDelegate);

UCLASS()
class SINKDOWN_API UAction_Node : public UObject, public IBehaviorTree_Node
{
	GENERATED_BODY()

private:
	FActionDelegate Action;
	
public:
	UAction_Node() {}
	UAction_Node(FActionDelegate InAction) : Action(InAction) {}

	virtual ENodeStatus Tick() override;
};
