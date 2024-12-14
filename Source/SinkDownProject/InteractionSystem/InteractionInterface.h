#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class SINKDOWNPROJECT_API IInteractionInterface
{
	GENERATED_BODY()

public:

	virtual void InteractionWithMe();

	virtual void ShowInteractionWidget();
	virtual void HideInteractionWidget();
};
