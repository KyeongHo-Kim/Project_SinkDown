#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageHandlerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDamageHandlerInterface : public UInterface
{
    GENERATED_BODY()
};

class SINKDOWNPROJECT_API IDamageHandlerInterface
{
    GENERATED_BODY()

public:
    virtual void Initialize(int32 InitialHealth) = 0;
    virtual void TakeDamage(bool bIsMelee) = 0;
    virtual bool IsDead() const = 0;
};