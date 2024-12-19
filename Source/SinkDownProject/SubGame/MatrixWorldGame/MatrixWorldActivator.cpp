#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldActivator.h"
#include "Components/StaticMeshComponent.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldGameComponent.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemy.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemyWayPoint.h"

AMatrixWorldActivator::AMatrixWorldActivator()
{
	InteractiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MatrixWorld Activator Mesh"));
	InteractiveMesh->SetupAttachment(RootComponent);

	MatrixWorldGameComponent = CreateDefaultSubobject<UMatrixWorldGameComponent>(TEXT("MatrixWorldGameComponent"));
}

void AMatrixWorldActivator::OnInteract()
{
	if (!MatrixWorldGameComponent || PathPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Required components or endpoints are missing"));
		return;
	}

	if (MatrixWorldGameComponent->GetCurrentState() != EMatrixWorldState::Inactive)
	{
		return;
	}

	GameSetup();
}

void AMatrixWorldActivator::GameSetup()
{
	MatrixWorldGameComponent->SetPathPoints(PathPoints);

	MatrixWorldGameComponent->StartGame();
}
