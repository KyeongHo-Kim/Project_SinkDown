#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldActivator.h"
#include "Components/StaticMeshComponent.h"

AMatrixWorldActivator::AMatrixWorldActivator()
{
	InteractiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MatrixWorld Activator Mesh"));
	InteractiveMesh->SetupAttachment(RootComponent);
}

void AMatrixWorldActivator::OnInteract()
{
}
