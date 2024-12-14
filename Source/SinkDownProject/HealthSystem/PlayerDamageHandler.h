#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageHandlerInterface.h"
#include "PlayerDamageHandler.generated.h"

UCLASS()
class SINKDOWNPROJECT_API UPlayerDamageHandler : public UObject, public IDamageHandlerInterface
{
	GENERATED_BODY()
	
public:
	UPlayerDamageHandler();

    virtual void Initialize(int32 InitialHealth) override;
    virtual void TakeDamage(bool bIsMelee) override;
    virtual bool IsDead() const override;

private:
    UPROPERTY() int32 MaxHealth;
    UPROPERTY() int32 CurrentHealth;

    UPROPERTY() bool bIsDead;
};
