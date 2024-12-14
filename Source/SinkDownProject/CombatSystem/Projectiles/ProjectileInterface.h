#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UProjectileInterface : public UInterface
{
	GENERATED_BODY()
};


class SINKDOWNPROJECT_API IProjectileInterface
{
	GENERATED_BODY()

public:
	virtual void Initialize(const FVector& StartLocation, const FVector& Direction) = 0;
	virtual void OnHit(AActor* HitActor, AActor* ProjectileOwner) = 0;
};
