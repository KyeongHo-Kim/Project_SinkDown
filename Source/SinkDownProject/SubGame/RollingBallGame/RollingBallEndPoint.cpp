#include "SinkDownProject/SubGame/RollingBallGame/RollingBallEndPoint.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ARollingBallEndPoint::ARollingBallEndPoint()
{
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootScene);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootScene);
}

void ARollingBallEndPoint::BeginPlay()
{
    Super::BeginPlay();
}

void ARollingBallEndPoint::StartPhysicsDestruction()
{
    if (!MeshComponent) return;


    MeshComponent->SetMassOverrideInKg(NAME_None, 3.0f, true);
    MeshComponent->SetSimulatePhysics(true);

    if (DestructionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation());
    }

    // Destroy after delay
    FTimerHandle DestroyTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        DestroyTimerHandle,
        [this]()
        {
            Destroy();
        },
        2.0f,
        false
    );
}