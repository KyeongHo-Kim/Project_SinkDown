#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatrixWorldEnemyWayPoint.generated.h"

class UStaticMeshComponent;

UCLASS()
class SINKDOWNPROJECT_API AMatrixWorldEnemyWayPoint : public AActor
{
	GENERATED_BODY()
	
public:	

	AMatrixWorldEnemyWayPoint();

protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Visualization")
	UStaticMeshComponent* PointMesh;

};
