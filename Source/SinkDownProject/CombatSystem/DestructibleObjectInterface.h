#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DestructibleObjectInterface.generated.h"


UINTERFACE(MinimalAPI)
class UDestructibleObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class SINKDOWNPROJECT_API IDestructibleObjectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Destruction") void Destroy();
};

// A class that implements this interface is a destructible object.