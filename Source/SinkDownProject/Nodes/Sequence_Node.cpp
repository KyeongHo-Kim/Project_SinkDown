#include "SinkDownProject/Nodes/Sequence_Node.h"

ENodeStatus USequence_Node::Tick()
{
	for (UBehaviorTree_Node* Child : GetChildren())
	{
		ENodeStatus Status = Child->Tick();
		if (Status != ENodeStatus::Success)
		{
			return Status;
		}
	}
	return ENodeStatus::Success; 
}
