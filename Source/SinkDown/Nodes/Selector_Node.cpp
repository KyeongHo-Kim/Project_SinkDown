#include "Selector_Node.h"

ENodeStatus USelector_Node::Tick()
{
    for (const TWeakObjectPtr<IBehaviorTree_Node>& WeakNode : Children)
    {
        if (IBehaviorTree_Node* Node = WeakNode.Get())
        {
            ENodeStatus Status = Node->Tick();
            if (Status != ENodeStatus::Failure)
            {
                return Status;
            }
        }
    }
    return ENodeStatus::Failure;
}