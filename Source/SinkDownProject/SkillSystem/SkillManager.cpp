#include "SinkDownProject/SkillSystem/SkillManager.h"
#include "SinkDownProject/SkillSystem/Melee/GlidingSkill.h"
#include "SinkDownProject/SkillSystem/Melee/ParrySkill.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/Items/Weapon/WeaponManager.h"
#include "SinkDownProject/SkillSystem/SkillEffectManager.h"


/**
 * Default constructor for the SkillManager component
 * Initializes a new instance of the skill management system
 */
USkillManager::USkillManager()
{

}

/**
 * Called when the game starts or when spawned
 * Initializes the skill system by calling InitializeSkills
 */
void USkillManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeSkills();
}

/**
 * Initializes all skills for the character
 * Creates and registers the GlidingSkill component
 * Sets up the connection between skills and the weapon manager
 */
void USkillManager::InitializeSkills()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	// Creating an Effects Manager
	EffectManager = NewObject<USkillEffectManager>(this);
	EffectManager->RegisterComponent();

	// Setting up effect data
	EffectManager->SetEffectData(EffectData);

	UGlidingSkill* GlidingSkill = NewObject<UGlidingSkill>(this);
	GlidingSkill->RegisterComponent();
	Skills.Add(GlidingSkill);

	UParrySkill* ParrySkill = NewObject<UParrySkill>(this);
	ParrySkill->RegisterComponent();
	Skills.Add(ParrySkill);

	WeaponManager = OwnerCharacter->FindComponentByClass<UWeaponManager>();
	if (WeaponManager)
	{
		WeaponManager->BindToSkill(GlidingSkill);
	}

	// Setting up an Effects Manager for each skill
	for (USkillBase* Skill : Skills)
	{
		Skill->SetEffectManager(EffectManager);
	}
}

/**
 * Activates a skill at the specified index
 * @param SkillIndex - The index of the skill to activate
 * Checks if the skill can be activated before triggering it
 */
void USkillManager::ActivateSkill(int32 SkillIndex)
{
	if (Skills.IsValidIndex(SkillIndex) && Skills[SkillIndex]->CanActivateSkill())
	{
		PlaySkillAnimation(Skills[SkillIndex]->GetSkillType());

		Skills[SkillIndex]->ActivateSkill(Cast<ACharacter>(GetOwner()));
	}
}

/**
 * Checks if any skill is currently active
 * @return bool - Returns true if any skill is active, false otherwise
 */
bool USkillManager::IsAnySkillActive() const
{
	for (USkillBase* Skill : Skills)
	{
		if (Skill && Skill->IsSkillActive())
		{
			return true;
		}
	}
	return false;
}

/**
 * Checks if a specific type of skill is active
 * @param SkillType - The type of skill to check
 * @return bool - Returns true if the specified skill type is active, false otherwise
 */
bool USkillManager::IsSkillActive(ESkillType SkillType) const
{
	for (USkillBase* Skill : Skills)
	{
		if (Skill && Skill->GetSkillType() == SkillType && Skill->IsSkillActive())
		{
			return true;
		}
	}
	return false;
}


void USkillManager::PlaySkillAnimation(ESkillType SkillType)
{
	if (!OwnerCharacter) return;

	if (UAnimMontage** FoundAnim = SkillAnimations.Find(SkillType))
	{
		if (*FoundAnim)
		{
			OwnerCharacter->PlayAnimMontage(*FoundAnim);
		}
	}
}