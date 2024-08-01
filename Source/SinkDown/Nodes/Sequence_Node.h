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
	TArray<IBehaviorTree_Node*> Children;

public:
	USequence_Node() {}
	USequence_Node(const TArray<IBehaviorTree_Node*>& Node) : Children(Node) {};

	virtual ENodeStatus Tick() override;
};
