#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/Enemys/Enemy.h"
#include "MeleeEnemy.generated.h"

UCLASS()
class SINKDOWNPROJECT_API AMeleeEnemy : public AEnemy
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	void Attack() override;
	float GetAttackRange() override;
	float GetDetectedRange() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation") UAnimMontage* MeleeAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation") UAnimMontage* MeleeDeathMontage;

	virtual void PlayAttackMontage() override;
	virtual float PlayDeathMontage() override;
};
