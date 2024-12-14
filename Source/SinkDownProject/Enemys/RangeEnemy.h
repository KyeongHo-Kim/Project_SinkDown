#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/Enemys/Enemy.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RangeEnemy.generated.h"

UCLASS()
class SINKDOWNPROJECT_API ARangeEnemy : public AEnemy
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    void Attack() override;
    float GetAttackRange() override;
    float GetDetectedRange() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* RangeAttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* RangeDeathMontage;

    virtual void PlayAttackMontage() override;
    virtual float PlayDeathMontage() override;

    void FireProjectile();

    // ��������Ʈ�� UFUNCTION �߰�
    UFUNCTION()
    void OnNotifyFireProjectile(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

    // �������Ʈ�� UFUNCTION
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void OnAnimNotify(FName NotifyName);
};