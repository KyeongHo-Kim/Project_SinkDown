#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/CombatSystem/Projectiles/BaseProjectile.h"
#include "MugunghwaProjectile.generated.h"


UCLASS()
class SINKDOWNPROJECT_API AMugunghwaProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	AMugunghwaProjectile();

protected:
	virtual void OnHit(AActor* HitActor, AActor* ProjectileOwner) override;

	float KnockbackForce;
	float UpwardForce;
};
