#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/SkillSystem/SkillBase.h"
#include "MeleeSkill.generated.h"

class UCharacterMovementComponent;

UCLASS()
class SINKDOWNPROJECT_API UMeleeSkill : public USkillBase
{
	GENERATED_BODY()

public:
	UMeleeSkill();

	virtual void ActivateSkill(ACharacter* Owner) override;
protected:
	UPROPERTY() UCharacterMovementComponent* CharacterMovement;
};
