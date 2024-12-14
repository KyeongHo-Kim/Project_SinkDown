#include "SinkDownProject/SubGame/SteppingStonesGame/FakeGround.h"
#include "Components/StaticMeshComponent.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"

AFakeGround::AFakeGround()
{
    PrimaryActorTick.bCanEverTick = false;

    GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground Mesh"));
    RootComponent = GroundMesh;

    GroundMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GroundMesh->SetCollisionResponseToAllChannels(ECR_Block);
    GroundMesh->SetMobility(EComponentMobility::Movable);

}

void AFakeGround::BeginPlay()
{
    Super::BeginPlay();

    GroundMesh->SetSimulatePhysics(false);
    GroundMesh->SetEnableGravity(true);

    OnActorHit.AddDynamic(this, &AFakeGround::OnHit);
}

void AFakeGround::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    if (ASinkDownProjectCharacter* Character = Cast<ASinkDownProjectCharacter>(OtherActor))
    {
        EnablePhysicsAndFall();
    }
}

void AFakeGround::EnablePhysicsAndFall()
{
    if (GroundMesh)
    {
        bIsPhysicsEnabled = true; 

        GroundMesh->SetSimulatePhysics(true);
        GroundMesh->SetMobility(EComponentMobility::Movable);

        GroundMesh->SetLinearDamping(0.0f);
        GroundMesh->SetAngularDamping(0.0f);

        GroundMesh->AddImpulse(FVector(0.0f, 0.0f, -4000.0f));

        FTimerHandle DestroyTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DestroyTimerHandle,
            this,
            &AFakeGround::DestroyGround,
            DestroyDelay,
            false
        );
    }
}

void AFakeGround::DestroyGround()
{
    Destroy();
}