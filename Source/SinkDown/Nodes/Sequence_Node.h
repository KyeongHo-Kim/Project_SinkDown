#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BehaviorTree_Node.h"
#include "Sequence_Node.generated.h"

UCLASS()
class SINKDOWN_API USequence_Node : public UObject, public IBehaviorTree_Node

{
	GENERATED_BODY()
	
private:
    TArray<TWeakObjectPtr<IBehaviorTree_Node>> Children;

public:
    USequence_Node() {}

    virtual ENodeStatus Tick() override;

    void AddChildNode(TWeakObjectPtr<IBehaviorTree_Node> Node) { Children.Add(Node); }
    void SetChildren(const TArray<TWeakObjectPtr<IBehaviorTree_Node>>& Nodes) { Children = Nodes; }
};
