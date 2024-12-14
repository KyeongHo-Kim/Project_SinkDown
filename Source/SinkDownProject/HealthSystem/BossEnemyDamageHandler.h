#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageHandlerInterface.h"
#include "BossEnemyDamageHandler.generated.h"

UCLASS()
class SINKDOWNPROJECT_API UBossEnemyDamageHandler : public UObject, public IDamageHandlerInterface
{
	GENERATED_BODY()
	
public:
    UBossEnemyDamageHandler();

    virtual void Initialize(int32 InitialHealth) override;
    virtual void TakeDamage(bool bIsMelee) override;
    virtual bool IsDead() const override;

private:
    UPROPERTY() int32 MaxHealth;
    UPROPERTY() int32 CurrentHealth;

    UPROPERTY() bool bIsDead;

};
