#include "SinkDownProject/Artefacts/BouncePad.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABouncePad::ABouncePad()
    : BoxExtent(FVector(50.0f, 50.0f, 50.0f))
    , BounceStrength(1500.0f)
{
    // Create and set root scene component
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    // Create trigger box and attach to root
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(SceneRoot);

    // Create static mesh and attach to root
    BouncePadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    BouncePadMesh->SetupAttachment(SceneRoot);

    // Setup trigger box properties
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerBox->SetBoxExtent(BoxExtent);

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABouncePad::OnOverlapBegin);
}

void ABouncePad::BeginPlay()
{
    Super::BeginPlay();
}

void ABouncePad::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActer, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* Character = Cast<ACharacter>(OtherActer);
    if (Character)
    {
        Character->LaunchCharacter(GetActorUpVector() * BounceStrength, true, true);

        if (BounceSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(this, BounceSoundCue, GetActorLocation(), 1.0f, 1.0f);
        }
    }
}