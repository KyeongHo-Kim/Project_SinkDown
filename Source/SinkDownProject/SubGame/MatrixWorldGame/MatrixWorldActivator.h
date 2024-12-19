#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "MatrixWorldActivator.generated.h"

class UStaticMeshComponent;
class UMatrixWorldGameComponent;
class AMatrixWorldEnemy;
class AMatrixWorldEnemyWayPoint;

UCLASS()
class SINKDOWNPROJECT_API AMatrixWorldActivator : public AInteractiveObjectBase
{
	GENERATED_BODY()

public:
	AMatrixWorldActivator();

protected:
	virtual void OnInteract() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh") UStaticMeshComponent* InteractiveMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Actor Reference") UMatrixWorldGameComponent* MatrixWorldGameComponent;
	UPROPERTY(EditAnywhere, Category = "Path Points") TArray<AMatrixWorldEnemyWayPoint*> PathPoints;

	void GameSetup();
};
