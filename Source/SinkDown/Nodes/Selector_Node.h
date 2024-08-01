#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BehaviorTree_Node.h"
#include "Selector_Node.generated.h"


UCLASS()
class SINKDOWN_API USelector_Node : public UObject, public IBehaviorTree_Node
{
	GENERATED_BODY()
	
private:
	TArray<IBehaviorTree_Node*> Children;

public:
	USelector_Node(){}
	USelector_Node(const TArray<IBehaviorTree_Node*>& Node) : Children(Node){}

	virtual ENodeStatus Tick() override;
};
