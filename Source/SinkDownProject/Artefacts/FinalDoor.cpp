#include "SinkDownProject/Artefacts/FinalDoor.h"
#include "SinkDownProject/GameLevels/DiarySubsystem.h"
#include "Kismet/GameplayStatics.h"

AFinalDoor::AFinalDoor()
{

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    RootComponent = DoorMesh;
}

void AFinalDoor::BeginPlay()
{
    Super::BeginPlay();

    // Register a delegate in the DiarySubsystem
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UDiarySubsystem* DiarySubsystem = GameInstance->GetSubsystem<UDiarySubsystem>())
        {
            DiarySubsystem->OnAllDiaryPiecesCollected.AddDynamic(this, &AFinalDoor::OnAllDiaryPiecesCollected);

            // Open the door if all fragments have already been collected
            if (DiarySubsystem->HasCollectedAllPieces())
            {
                OpenDoor();
            }
        }
    }
}

void AFinalDoor::OnAllDiaryPiecesCollected()
{
    OpenDoor();
}

void AFinalDoor::OpenDoor()
{

    if (DoorOpenSound)
    {
        UGameplayStatics::PlaySound2D(this, DoorOpenSound);
    }

    DoorMesh->SetRelativeRotation(FRotator(0.0f, OpenAngle, 0.0f));
}