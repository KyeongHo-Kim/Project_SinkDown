#include "Sequence_Node.h"

ENodeStatus USequence_Node::Tick()
{
    for (const TWeakObjectPtr<IBehaviorTree_Node>& WeakNode : Children)
    {
        if (IBehaviorTree_Node* Node = WeakNode.Get())
        {
            ENodeStatus Status = Node->Tick();
            if (Status != ENodeStatus::Success)
            {
                return Status;
            }
        }
    }
    return ENodeStatus::Success;

}