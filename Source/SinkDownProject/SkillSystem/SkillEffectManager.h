#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SinkDownProject/SkillSystem/SkillTypes.h"
#include "SkillEffectManager.generated.h"

class USkillEffectData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API USkillEffectManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillEffectManager();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true")) USkillEffectData* EffectData;

public:
	void PlaySkillEffects(ESkillType SkillType, const FVector& Location);

	void SetEffectData(USkillEffectData* InEffectData) { EffectData = InEffectData; }
};
