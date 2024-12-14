#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BehaviorTree_Interface.generated.h"

UENUM(BlueprintType)
enum class ENodeStatus : uint8
{
	Success UMETA(DisplayName = "Success"),
	Failure UMETA(DisplayName = "Failure"),
	Running UMETA(DisplayName = "Running")
};

UINTERFACE(MinimalAPI)
class UBehaviorTree_Interface : public UInterface
{
	GENERATED_BODY()
};


class SINKDOWNPROJECT_API IBehaviorTree_Interface
{
	GENERATED_BODY()

public:
	virtual ENodeStatus Tick() = 0;
};
