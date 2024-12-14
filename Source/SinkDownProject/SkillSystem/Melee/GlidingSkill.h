#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/SkillSystem/Melee/MeleeSkill.h"
#include "GlidingSkill.generated.h"

class ACharacter;

UCLASS()
class SINKDOWNPROJECT_API UGlidingSkill : public UMeleeSkill
{
    GENERATED_BODY()

public:
    UGlidingSkill();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void ActivateSkill(ACharacter* Owner) override;
    virtual bool UsesCooldown() const override { return false; }
    virtual bool CanActivateSkill() const override { return true; }
    virtual bool IsSkillActive() const override { return bIsGliding; }
    virtual ESkillType GetSkillType() const override { return ESkillType::Gliding; }

protected:
    // Constants
    static constexpr float DEFAULT_ROTATION_RATE_YAW = 250.0f;
    static constexpr float DEFAULT_AIR_CONTROL = 0.9f;
    static constexpr float DEFAULT_BRAKING_DECELERATION = 350.0f;
    static constexpr float DEFAULT_MAX_ACCELERATION = 1024.0f;
    static constexpr float DEFAULT_MAX_WALK_SPEED = 600.0f;
    static constexpr float DEFAULT_INTERPOLATION_SPEED = 3.0f;

    bool bIsGliding;
    FVector CurrentVelocity;

    UPROPERTY(EditAnywhere, Category = "Gliding") float MinimumHeight;
    UPROPERTY(EditAnywhere, Category = "Gliding") float DescendingRate;

    bool OriginalOrientRotation;
    float OriginalGravityScale;
    float OriginalWalkingSpeed;
    float OriginalDecelration;
    float OriginalAcceleration;
    float OriginalAirControl;
    bool OriginalDesiredRotation;

    void StartGliding();
    void StopGliding();
    bool CanStartGliding();
    void RecordOriginalSetting();
    void DescentPlayer();
    void ApplyOriginalSettings();
};