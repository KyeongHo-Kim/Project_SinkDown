#include "SinkDownProject/Artefacts/EndingDoor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownProject/GameLevels/DiarySubsystem.h"
#include "SinkDownProject/SinkDownProjectGameMode.h"


AEndingDoor::AEndingDoor()
{
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    DoorFrame->SetupAttachment(RootScene);

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    Door->SetupAttachment(RootScene);
}


void AEndingDoor::BeginPlay()
{
	Super::BeginPlay();
	
    if (TriggerVolume)
    {
        TriggerVolume->OnActorBeginOverlap.AddDynamic(this, &AEndingDoor::OnOverlapBegin);
    }
}

void AEndingDoor::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!Cast<ACharacter>(OtherActor)) return;

    bool canTriggerEnding = true;

    if (bRequireAllDiaries)
    {
        if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
        {
            if (UDiarySubsystem* DiarySubsystem = GameInstance->GetSubsystem<UDiarySubsystem>())
            {
                canTriggerEnding = DiarySubsystem->HasCollectedAllPieces();
            }
        }
    }

    if (canTriggerEnding)
    {
        if (ASinkDownProjectGameMode* GameMode = Cast<ASinkDownProjectGameMode>(UGameplayStatics::GetGameMode(this)))
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle,
                [GameMode]()
                {
                    GameMode->TransitionToMainMenu();
                },
                0.5f, false);
        }
    }
}