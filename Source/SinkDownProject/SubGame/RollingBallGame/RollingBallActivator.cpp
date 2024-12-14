#include "SinkDownProject/SubGame/RollingBallGame/RollingBallActivator.h"
#include "Components/StaticMeshComponent.h"
#include "SinkDownProject/SubGame/RollingBallGame/RollingBallGameComponent.h"
#include "SinkDownProject/SubGame/RollingBallGame/RollingBallEndPoint.h"

ARollingBallActivator::ARollingBallActivator()
{
	InteractiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RollingBall Activator Mesh"));
	InteractiveMesh->SetupAttachment(RootComponent);

	RollingBallGameComponent = CreateDefaultSubobject<URollingBallGameComponent>(TEXT("RollingBallGameComponent"));
}


void ARollingBallActivator::OnInteract()
{
    if (!RollingBallGameComponent || !TargetEndPoint)
    {
        UE_LOG(LogTemp, Warning, TEXT("Required components or endpoints are missing"));
        return;
    }

    if (RollingBallGameComponent->GetCurrentState() == ERollingBallState::Completed ||
        RollingBallGameComponent->GetCurrentState() == ERollingBallState::Failed)
    {
        return;
    }

    GameSetup();
}

void ARollingBallActivator::GameSetup()
{
    RollingBallGameComponent->SetTargetEndPoint(TargetEndPoint);
    RollingBallGameComponent->StartGame();
}
