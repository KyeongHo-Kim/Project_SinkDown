#include "Selector_Node.h"

ENodeStatus USelector_Node::Tick()
{
	for (IBehaviorTree_Node* Node : Children)
	{
		ENodeStatus Status = Node->Tick();
		if (Status != ENodeStatus::Failure)
		{
			return Status;
		}
	}
	return ENodeStatus::Failure;
}
