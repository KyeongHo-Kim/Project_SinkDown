#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/Nodes/BehaviorTree_Node.h"
#include "Selector_Node.generated.h"

UCLASS()
class SINKDOWNPROJECT_API USelector_Node : public UBehaviorTree_Node
{
	GENERATED_BODY()
	
public:
	virtual ENodeStatus Tick() override;
};
