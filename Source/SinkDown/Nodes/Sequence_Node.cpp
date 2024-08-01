#include "Sequence_Node.h"

ENodeStatus USequence_Node::Tick()
{
    for (IBehaviorTree_Node* Node : Children)
    {
        ENodeStatus Status = Node->Tick();
        if (Status != ENodeStatus::Success)
        {
            return Status;
        }
    }
    return ENodeStatus::Success;
}
