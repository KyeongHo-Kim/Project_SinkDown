#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManager.generated.h"

class UWeaponData;
class AWeapon;
class USkillBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponManager();

protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY() TArray<UWeaponData*> AvailableWeapons;
    UPROPERTY() TMap<EWeaponType, AWeapon*> EquippedWeapons;
    UPROPERTY() EWeaponType ActiveWeaponType;

    AWeapon* SpawnWeapon(UWeaponData* WeaponData);
    void SetActiveWeapon(EWeaponType NewWeaponType);


    void OnSkillStateChangedHandler(bool bActive);

public:	
    void AddWeapon(UWeaponData* WeaponData);
    void EquipWeapon(EWeaponType WeaponType);

    void DeactivateCurrentWeapon();
    void SwitchWeapon();
    void UpdateWeaponsVisibility();

    UFUNCTION(BlueprintCallable, Category = "Weapon") TArray<UWeaponData*> GetAvailableWeapons() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon") UWeaponData* GetCurrentWeapon() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon") bool IsWeaponEquipped(EWeaponType WeaponType) const;

    UFUNCTION(BlueprintCallable, Category = "Weapon") EWeaponType GetActiveWeaponType() const;

    FVector GetCurrentWeaponLocation() const;


    void BindToSkill(USkillBase* Skill);
    void SetCurrentWeaponGlidingState(bool bUseGlidingMesh);
};
