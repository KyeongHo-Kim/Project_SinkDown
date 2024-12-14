#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaDoor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AMugunghwaDoor::AMugunghwaDoor()
{
    PrimaryActorTick.bCanEverTick = true;

    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootScene);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootScene);

    bIsOpening = false;
}

void AMugunghwaDoor::BeginPlay()
{
    Super::BeginPlay();
    InitialRotation = GetActorRotation();
    TargetRotation = InitialRotation;
    TargetRotation.Yaw += TargetYaw;
}

void AMugunghwaDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsOpening)
    {
        FRotator CurrentRotation = GetActorRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, OpenSpeed);
        SetActorRotation(NewRotation);

        if (FMath::IsNearlyEqual(NewRotation.Yaw, TargetRotation.Yaw, 0.1f))
        {
            bIsOpening = false;
        }
    }
}

void AMugunghwaDoor::OpenDoor()
{
    bIsOpening = true;
}