#include "SinkDownProject/Nodes/Action_Node.h"

ENodeStatus UAction_Node::Tick()
{
	if (Action.IsBound())
	{
		return Action.Execute();
	}
	return ENodeStatus::Failure;
}
