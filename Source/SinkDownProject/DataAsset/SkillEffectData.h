#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SinkDownProject/SkillSystem/SkillTypes.h"
#include "SkillEffectData.generated.h"

class UNiagaraSystem;
class USoundBase;

USTRUCT(BlueprintType)
struct FSkillEffectInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraSystem* Effect;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundBase* Sound;

    UPROPERTY(EditAnywhere, Category = "Effects")
    float ParticleScale = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Effects")
    float VolumeMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Effects")
    float PitchMultiplier = 1.0f;
};

UCLASS()
class SINKDOWNPROJECT_API USkillEffectData : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "Skill Effects") TMap<ESkillType, FSkillEffectInfo> SkillEffects;
};
