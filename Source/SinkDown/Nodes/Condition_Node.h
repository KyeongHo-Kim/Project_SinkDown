#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BehaviorTree_Node.h"
#include "Condition_Node.generated.h"

// FConditionDelegate라는 이름의 델리게이트를 선언
// 델리게이트는 bool 타입의 값을 반환하는 함수 시그니처를 가짐
DECLARE_DELEGATE_RetVal(bool, FConditionDelegate)

UCLASS()
class SINKDOWN_API UCondition_Node : public UObject, public IBehaviorTree_Node
{
	GENERATED_BODY()

private:
	FConditionDelegate Condition;

public:
	UCondition_Node() {}
	UCondition_Node(FConditionDelegate InCondition) : Condition(InCondition) {}

	virtual ENodeStatus Tick() override;
	
};
