#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaObstacle.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

AMugunghwaObstacle::AMugunghwaObstacle()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    MugunghwaObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MugunghwaObstacleMesh"));
    MugunghwaObstacleMesh->SetupAttachment(SceneRoot);

    MugunghwaObstacleMesh->SetSimulatePhysics(false);
    MugunghwaObstacleMesh->SetLinearDamping(0.5f);

    MugunghwaObstacleMesh->BodyInstance.bLockXRotation = true;
    MugunghwaObstacleMesh->BodyInstance.bLockYRotation = true;
}

void AMugunghwaObstacle::BeginPlay()
{
    Super::BeginPlay();

    if (TriggerVolume)
    {
        TriggerVolume->OnActorEndOverlap.AddDynamic(this, &AMugunghwaObstacle::OnOverlapEnd);
    }

    if (MugunghwaObstacleMesh)
    {
        MugunghwaObstacleMesh->SetNotifyRigidBodyCollision(true);
        MugunghwaObstacleMesh->OnComponentHit.AddDynamic(this, &AMugunghwaObstacle::OnHit);
    }
}

void AMugunghwaObstacle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMugunghwaObstacle::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character && Character->IsLocallyControlled())
    {
        MugunghwaObstacleMesh->SetSimulatePhysics(true);

        MugunghwaObstacleMesh->AddImpulse(FVector(0.0f, 0.0f, -FallSpeed));
    }
    if (ImpactSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
    }
}

void AMugunghwaObstacle::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    MugunghwaObstacleMesh->SetSimulatePhysics(false);
    SetActorTickEnabled(false);
}