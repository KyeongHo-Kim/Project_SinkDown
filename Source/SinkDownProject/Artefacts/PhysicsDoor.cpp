#include "SinkDownProject/Artefacts/PhysicsDoor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

APhysicsDoor::APhysicsDoor()
{
    // Create and set root component
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    // Create door frame
    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    DoorFrame->SetupAttachment(RootScene);

    // Create door
    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    Door->SetupAttachment(RootScene);
}

void APhysicsDoor::BeginPlay()
{
    Super::BeginPlay();

    // If TriggerVolume is set, bind the overlap event to the
    if (TriggerVolume)
    {
        TriggerVolume->OnActorBeginOverlap.AddDynamic(this, &APhysicsDoor::OnOverlapBegin);
    }
}

void APhysicsDoor::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character && DestinationDoor)
    {
        FVector TeleportLocation = DestinationDoor->GetActorLocation() + TeleportOffset;
        FRotator TeleportRotation = DestinationDoor->GetActorRotation();

        Character->SetActorLocationAndRotation(TeleportLocation, TeleportRotation);
    }
}