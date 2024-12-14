#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/SkillSystem/Melee/MeleeSkill.h"
#include "ParrySkill.generated.h"

class USphereComponent;

UCLASS()
class SINKDOWNPROJECT_API UParrySkill : public UMeleeSkill
{
	GENERATED_BODY()
	
public:
	UParrySkill();

protected:
	virtual void BeginPlay() override;

public:
	virtual void ActivateSkill(ACharacter* Owner) override;
	virtual bool IsSkillActive() const override { return bIsParrying; }
	virtual ESkillType GetSkillType() const override { return ESkillType::Parry; }

private:
    UPROPERTY() USphereComponent* ParryCollision;
    UPROPERTY() float ParryDuration;
    UPROPERTY() float ReducedCooldownTime;
    UPROPERTY()  float ParryRadius;
    UPROPERTY() bool bIsParrying;
    FTimerHandle ParryTimerHandle;

    UFUNCTION() void OnParryEnd();
    UFUNCTION() void HandleProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void StartParryWindow();
    void SetupParryCollision();

};
