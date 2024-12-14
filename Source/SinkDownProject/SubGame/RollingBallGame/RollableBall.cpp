#include "SinkDownProject/SubGame/RollingBallGame/RollableBall.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/SubGame/RollingBallGame/RollingBallEndPoint.h"

ARollableBall::ARollableBall()
{
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;

    BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BallMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
    BallMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void ARollableBall::BeginPlay()
{
    Super::BeginPlay();


    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetEnableGravity(true);
    BallMesh->SetNotifyRigidBodyCollision(true);
    BallMesh->CanCharacterStepUpOn = ECB_No;  

    BallMesh->GetBodyInstance()->SetMassScale(1.0f); 
    BallMesh->SetAngularDamping(0.1f); 
    BallMesh->SetLinearDamping(0.1f);

    OnActorHit.AddDynamic(this, &ARollableBall::OnHit);
}

void ARollableBall::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    // Character pushing ball logic
    if (ACharacter* Character = Cast<ACharacter>(OtherActor))
    {
        FVector CharacterVelocity = Character->GetVelocity();
        if (!CharacterVelocity.IsNearlyZero())
        {
            CharacterVelocity.Z = 0;
            BallMesh->SetPhysicsLinearVelocity(CharacterVelocity);
        }
    }

    // EndPoint collision check
    if (TargetEndPoint && OtherActor == TargetEndPoint)
    {
        OnReachedEndPoint.Broadcast();
    }
}