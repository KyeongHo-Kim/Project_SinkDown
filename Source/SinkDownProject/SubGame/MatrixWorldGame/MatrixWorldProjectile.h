
#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/CombatSystem/Projectiles/BaseProjectile.h"
#include "MatrixWorldProjectile.generated.h"

UCLASS()
class SINKDOWNPROJECT_API AMatrixWorldProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	AMatrixWorldProjectile();

	virtual void Tick(float DeltaTime) override;
	virtual void Initialize(const FVector& StartLocation, const FVector& Direction) override;
	virtual void OnHit(AActor* HitActor, AActor* ProjectileOwner) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile") float TurnSpeed = 3.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Projectile") float ProjectileSpeed = 1000.0f;

private:
	UPROPERTY() AActor* TargetActor;

    AActor* FindTarget() const;
    void UpdateProjectileDirection(float DeltaTime);
};
