#include "SinkDownProject/Nodes/Condition_Node.h"

ENodeStatus UCondition_Node::Tick()
{
	return Condition.Execute() ? ENodeStatus::Success : ENodeStatus::Failure;
}
