#include "SinkDownProject/SoundSystem/SoundZoneTrigger.h"
#include "SinkDownProject/SoundSystem/GameSoundManager.h"
#include "Kismet/GameplayStatics.h"

ASoundZoneTrigger::ASoundZoneTrigger()
{
    OnActorBeginOverlap.AddDynamic(this, &ASoundZoneTrigger::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ASoundZoneTrigger::OnOverlapEnd);
}

void ASoundZoneTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        AGameSoundManager* SoundManager = Cast<AGameSoundManager>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AGameSoundManager::StaticClass())
        );

        if (SoundManager)
        {
            SoundManager->EnterZone(ZoneName);
        }
    }
}
void ASoundZoneTrigger::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        AGameSoundManager* SoundManager = Cast<AGameSoundManager>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AGameSoundManager::StaticClass())
        );

        if (SoundManager)
        {
            SoundManager->ReturnToBaseBGM();
        }
    }
}