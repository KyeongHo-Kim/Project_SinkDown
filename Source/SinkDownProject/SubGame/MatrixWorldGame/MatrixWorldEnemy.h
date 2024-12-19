#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MatrixWorldEnemy.generated.h"

class AMatrixWorldEnemyWayPoint;
class AMatrixWorldProjectile;
class AHomingProjectile;


UCLASS()
class SINKDOWNPROJECT_API AMatrixWorldEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AMatrixWorldEnemy();
	virtual void Tick(float DeltaTime) override;
protected:

	virtual void BeginPlay() override;

protected:
	UPROPERTY() TArray<AMatrixWorldEnemyWayPoint*> PathPoints;
	int32 CurrentPathIndex;

	void MoveToNextPoint();
	void OnReachedPathPoint();

	// Projectile Related
	UPROPERTY(EditDefaultsOnly, Category = "Projectile") TSubclassOf<AMatrixWorldProjectile> MatrixWorldProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile") TSubclassOf<AHomingProjectile> HomingProjectileClass;
	FTimerHandle HomingProjectileTimerHandle;
	FTimerHandle MatrixProjectileTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile") float HomingProjectileInterval = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile") float MatrixProjectileInterval = 5.0f;

	void FireHomingProjectile();
	void FireMatrixProjectile();

public:	
	void InitializePathPoints(const TArray<AMatrixWorldEnemyWayPoint*>& Points);
};
