#include "SinkDownProject/SkillSystem/SkillEffectManager.h"
#include "SinkDownProject/DataAsset/SkillEffectData.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"

USkillEffectManager::USkillEffectManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillEffectManager::BeginPlay()
{
	Super::BeginPlay();
}

void USkillEffectManager::PlaySkillEffects(ESkillType SkillType, const FVector& Location)
{
    if (!EffectData) return;

    const FSkillEffectInfo* EffectInfo = EffectData->SkillEffects.Find(SkillType);
    if (!EffectInfo) return;

    if (EffectInfo->Effect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            EffectInfo->Effect,
            Location,
            FRotator::ZeroRotator,
            FVector(EffectInfo->ParticleScale),
            true,
            true,
            ENCPoolMethod::AutoRelease,
            true
        );
    }

    if (EffectInfo->Sound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EffectInfo->Sound,
            Location,
            EffectInfo->VolumeMultiplier,
            EffectInfo->PitchMultiplier,
            0.0f,
            nullptr,
            nullptr
        );
    }
}


