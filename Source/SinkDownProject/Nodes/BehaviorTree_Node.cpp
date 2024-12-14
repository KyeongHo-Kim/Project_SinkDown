#include "SinkDownProject/Nodes/BehaviorTree_Node.h"

ENodeStatus UBehaviorTree_Node::Tick()
{
	return ENodeStatus::Failure;
}

void UBehaviorTree_Node::AddChild(UBehaviorTree_Node* ChildNode)
{
    if (ChildNode != nullptr)
    {
        Children.Add(ChildNode);
    }
}

const TArray<UBehaviorTree_Node*>& UBehaviorTree_Node::GetChildren() const
{
    return Children;
}
