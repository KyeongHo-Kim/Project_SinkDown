#include "SinkDownProject/CombatSystem/CombatEffectManager.h"
#include "SinkDownProject/DataAsset/CombatEffectData.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UCombatEffectManager::UCombatEffectManager()
{
}

void UCombatEffectManager::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatEffectManager::PlayMeleeHitEffect(const FVector& HitLocation, const FVector& HitNormal)
{
	if (!EffectData) return;

    //Functions that spawn a particle system at a specific location in the world.
	if (EffectData->MeleeEffect.HitEffect)
	{
		FRotator EffectRotation = HitNormal.Rotation();

        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            EffectData->MeleeEffect.HitEffect,
            HitLocation,
            EffectRotation,
            EffectData->MeleeEffect.EffectScale,
            true
        );
    }

    // Playing a blow sound
    PlaySoundAtLocation(EffectData->MeleeEffect.HitSound, HitLocation, EffectData->MeleeEffect.HitSoundVolume);
}

void UCombatEffectManager::PlayRangedFireSound()
{
    if (!EffectData) return;

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // Play firing sound (from owner's location)
    PlaySoundAtLocation(EffectData->RangedEffect.FireSound, Owner->GetActorLocation(), EffectData->RangedEffect.FireSoundVolume);
}

void UCombatEffectManager::PlayRangedHitSound(const FVector& HitLocation)
{
    if (!EffectData) return;

    PlaySoundAtLocation(EffectData->RangedEffect.HitSound, HitLocation, EffectData->RangedEffect.HitSoundVolume);
}

void UCombatEffectManager::PlaySoundAtLocation(USoundBase* Sound, const FVector& Location, float Volume)
{
    if (Sound && GetWorld())
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,           // WorldContextObject
            Sound,          // Sound
            Location,       // Location
            Volume,         // VolumeMultiplier
            1.0f,          // PitchMultiplier
            0.0f,          // StartTime
            nullptr,        // AttenuationSettings
            nullptr         // ConcurrencySettings
        );
    }
}

