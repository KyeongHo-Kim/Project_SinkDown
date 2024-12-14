#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatEffectManager.generated.h"

class UCombatEffectData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API UCombatEffectManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatEffectManager();

protected:
	virtual void BeginPlay() override;

public:	
	// Melee Attack Strike Effect
	void PlayMeleeHitEffect(const FVector& HitLocation, const FVector& HitNormal);

	// Ranged attack effects
	void PlayRangedFireSound();
	void PlayRangedHitSound(const FVector& HitLocation);
		
	UFUNCTION(BlueprintCallable, Category = "Combat|FX") void SetEffectData(UCombatEffectData* NewEffectData) { EffectData = NewEffectData; }


private:
	UPROPERTY(EditAnywhere, Category = "Combat|FX", meta = (AllowPrivateAccess = "true")) UCombatEffectData* EffectData;

	void PlaySoundAtLocation(USoundBase* Sound, const FVector& Location, float Volume);
};
