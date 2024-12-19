#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemyWayPoint.h"
#include "Components/StaticMeshComponent.h"

AMatrixWorldEnemyWayPoint::AMatrixWorldEnemyWayPoint()
{
	PointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointMesh"));
	RootComponent = PointMesh;
}

void AMatrixWorldEnemyWayPoint::BeginPlay()
{
	Super::BeginPlay();
}

