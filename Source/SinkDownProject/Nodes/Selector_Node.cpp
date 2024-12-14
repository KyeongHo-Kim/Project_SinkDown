#include "SinkDownProject/Nodes/Selector_Node.h"

ENodeStatus USelector_Node::Tick()
{
	for (UBehaviorTree_Node* Child : GetChildren())
	{
		ENodeStatus Status = Child->Tick();
		if (Status == ENodeStatus::Success)
		{
			return Status; 
		}
	}
	return ENodeStatus::Failure; 
}
