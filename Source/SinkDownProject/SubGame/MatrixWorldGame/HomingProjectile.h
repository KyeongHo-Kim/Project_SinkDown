#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingProjectile.generated.h"

class UStaticMeshComponent;

UCLASS()
class SINKDOWNPROJECT_API AHomingProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AHomingProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh") UStaticMeshComponent* ProjectileMesh;
	UPROPERTY() AActor* TargetActor;

	FTimerHandle DelayTrackingHandle;

	void FindTarget();
	void HomingBehavior(float DeltaTime);

	UFUNCTION() void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile") float Speed = 500.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile") float TurnRate = 120.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile") float HomingDelay = 0.5f;

};
