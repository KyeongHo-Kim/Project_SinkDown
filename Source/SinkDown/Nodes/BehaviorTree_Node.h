#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BehaviorTree_Node.generated.h"

enum class ENodeStatus : uint8
{
    Success UMETA(DisplayName = "Success"),
    Failure UMETA(DisplayName = "Failure"),
    Running UMETA(DisplayName = "Running")
};

UINTERFACE(MinimalAPI)
class UBehaviorTree_Node : public UInterface
{
    GENERATED_BODY()
};

class SINKDOWN_API IBehaviorTree_Node
{
    GENERATED_BODY()  

public:
    virtual ENodeStatus Tick() = 0;
};
