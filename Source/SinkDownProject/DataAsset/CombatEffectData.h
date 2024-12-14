#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatEffectData.generated.h"

class UNiagaraSystem;
class USoundBase;

USTRUCT(BlueprintType)
struct FMeleeEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* HitSound;
    UPROPERTY(EditAnywhere, Category = "Visual") UNiagaraSystem* HitEffect;
    UPROPERTY(EditAnywhere, Category = "Visual") FVector EffectScale = FVector(1.0f);
    UPROPERTY(EditAnywhere, Category = "Sound", meta = (ClampMin = "0.0", ClampMax = "1.0")) float HitSoundVolume = 1.0f;
};

USTRUCT(BlueprintType)
struct FRangedEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* FireSound;
    UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* HitSound;
    UPROPERTY(EditAnywhere, Category = "Sound", meta = (ClampMin = "0.0", ClampMax = "1.0")) float FireSoundVolume = 1.0f;
    UPROPERTY(EditAnywhere, Category = "Sound", meta = (ClampMin = "0.0", ClampMax = "1.0")) float HitSoundVolume = 1.0f;
};


UCLASS()
class SINKDOWNPROJECT_API UCombatEffectData : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Melee") FMeleeEffect MeleeEffect;
    UPROPERTY(EditAnywhere, Category = "Ranged") FRangedEffect RangedEffect;
	
};
