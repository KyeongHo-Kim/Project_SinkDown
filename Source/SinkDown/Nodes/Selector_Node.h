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
    TArray<TWeakObjectPtr<IBehaviorTree_Node>> Children;

public:
    USelector_Node() {}

    virtual ENodeStatus Tick() override;

    void AddChildNode(TWeakObjectPtr<IBehaviorTree_Node> Node) { Children.Add(Node); }
    void SetChildren(const TArray<TWeakObjectPtr<IBehaviorTree_Node>>& Nodes) { Children = Nodes; }
};
