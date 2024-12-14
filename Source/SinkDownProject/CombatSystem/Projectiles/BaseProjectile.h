#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileInterface.h"
#include "BaseProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProjectileHit, AActor*, HitActor, AActor*, ProjectileOwner, FVector, HitLocation);

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class SINKDOWNPROJECT_API ABaseProjectile : public AActor, public IProjectileInterface
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void Tick(float DeltaTime) override;

	virtual void Initialize(const FVector& StartLocation, const FVector& Direction) override;
	virtual void OnHit(AActor* HitActor, AActor* ProjectileOwner) override;

	UPROPERTY(BlueprintAssignable, Category = "Projectile") FOnProjectileHit OnProjectileHit;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")USphereComponent* CollisionComponent;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile") UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION() void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
