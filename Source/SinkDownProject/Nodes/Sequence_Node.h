#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/Nodes/BehaviorTree_Node.h"
#include "Sequence_Node.generated.h"

UCLASS()
class SINKDOWNPROJECT_API USequence_Node : public UBehaviorTree_Node
{
	GENERATED_BODY()
	
public:
	virtual ENodeStatus Tick() override;
};
