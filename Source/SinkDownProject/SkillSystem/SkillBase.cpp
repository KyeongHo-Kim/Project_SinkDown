#include "SinkDownProject/SkillSystem/SkillBase.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/SkillSystem/SkillEffectManager.h"

USkillBase::USkillBase()
{
	CooldownTime = 1.0f;
	CurrentCooldown = 0.0f;
	OwnerCharacter = nullptr;
}

void USkillBase::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void USkillBase::ActivateSkill(ACharacter* Owner)
{
	if (CanActivateSkill())
	{
		OwnerCharacter = Owner;
		if (UsesCooldown())
		{
			StartCooldown();
		}
	}
}

bool USkillBase::CanActivateSkill() const
{
	return !UsesCooldown() || CurrentCooldown <= 0.0f && !IsSkillActive();
}

void USkillBase::StartCooldown()
{
	if (CooldownTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}

	CurrentCooldown = CooldownTime;
	
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, [this]() {CurrentCooldown = 0.0f; }, CooldownTime, false);
}

void USkillBase::PlaySkillEffects(const FVector& Location) const
{
	if (EffectManager)
	{
		EffectManager->PlaySkillEffects(GetSkillType(), Location);
	}
}
