#include "SinkDownProject/SkillSystem/Melee/MeleeSkill.h"
#include "GameFramework/Character.h"

UMeleeSkill::UMeleeSkill()
{

}

void UMeleeSkill::ActivateSkill(ACharacter* Owner)
{
	Super::ActivateSkill(Owner);

    if (Owner)
    {
        CharacterMovement = Owner->GetCharacterMovement();
    }
}

