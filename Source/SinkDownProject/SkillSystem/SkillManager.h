#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SinkDownProject/SkillSystem/SkillTypes.h"
#include "SkillManager.generated.h"

class USkillBase;
class UWeaponManager;
class ACharacter;
class UAnimMontage;
class USkillEffectData;
class USkillEffectManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API USkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillManager();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY() TArray<USkillBase*> Skills;
	UPROPERTY() UWeaponManager* WeaponManager;
	UPROPERTY() ACharacter* OwnerCharacter;
	UPROPERTY() USkillEffectManager* EffectManager;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true")) TMap<ESkillType, UAnimMontage*> SkillAnimations;
	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true")) USkillEffectData* EffectData;

	void InitializeSkills();
	void PlaySkillAnimation(ESkillType SkillType);

public:
	UFUNCTION() void ActivateSkill(int32 SkillIndex);
	bool IsAnySkillActive() const;
	bool IsSkillActive(ESkillType SkillType) const;


	USkillEffectManager* GetEffectManager() const { return EffectManager; }
};
