#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SinkDownProject/SkillSystem/SkillTypes.h"
#include "SkillBase.generated.h"

class USkillEffectManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillStateChanged, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API USkillBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillBase();

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill") float CooldownTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill") float CurrentCooldown;
	FTimerHandle CooldownTimerHandle;
	UPROPERTY() ACharacter* OwnerCharacter;
	UPROPERTY() USkillEffectManager* EffectManager;

	virtual void StartCooldown();
	void PlaySkillEffects(const FVector& Location) const;

public:
	UFUNCTION(BlueprintCallable, Category = "Skill") virtual void ActivateSkill(ACharacter* Owner);
	UFUNCTION(BlueprintCallable, Category = "Skill") virtual bool CanActivateSkill() const;

	virtual bool UsesCooldown() const { return true; }
	virtual bool IsSkillActive() const { return false; }
	virtual ESkillType GetSkillType() const { return ESkillType::None; }
	void SetEffectManager(USkillEffectManager* InEffectManager) { EffectManager = InEffectManager; }

	FOnSkillStateChanged OnSkillStateChanged;
};
