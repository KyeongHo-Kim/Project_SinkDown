#include "SinkDownProject/SubGame/SteppingStonesGame/SteppingStoneActivator.h"
#include "SinkDownProject/SubGame/SteppingStonesGame/SteppingStoneGameComponent.h"
#include "Components/StaticMeshComponent.h"

ASteppingStoneActivator::ASteppingStoneActivator()
{
	InteractiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RollingBall Activator Mesh"));
	InteractiveMesh->SetupAttachment(RootComponent);

	SteppingStoneGameComponent = CreateDefaultSubobject<USteppingStoneGameComponent>(TEXT("SteppingStoneGameComponent"));
}

void ASteppingStoneActivator::OnInteract()
{
	if (SteppingStoneGameComponent)
	{
		SteppingStoneGameComponent->StartGame();
	}
}
