#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SinkDownProject/Nodes/BehaviorTree_Interface.h"
#include "BehaviorTree_Node.generated.h"

UCLASS()
class SINKDOWNPROJECT_API UBehaviorTree_Node : public UObject, public IBehaviorTree_Interface
{
	GENERATED_BODY()

public:
	virtual ENodeStatus Tick() override;

protected:
	UPROPERTY()
	TArray<UBehaviorTree_Node*> Children;

public:
	void AddChild(UBehaviorTree_Node* ChildNode);
	const TArray<UBehaviorTree_Node*>& GetChildren() const;
};
