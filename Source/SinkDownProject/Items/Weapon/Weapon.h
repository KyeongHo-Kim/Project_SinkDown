#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UWeaponData;
class USkeletalMeshComponent;

UCLASS()
class SINKDOWNPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();

protected:
	virtual void BeginPlay() override;

private:
	bool bIsActive;

	UPROPERTY(VisibleAnywhere) USkeletalMeshComponent* WeaponMesh;
	UPROPERTY() UWeaponData* WeaponData;

	void UpdateWeaponMesh();
	void SetupCollision();

public:
	void SetWeaponData(UWeaponData* NewWeaponData);
	UWeaponData* GetWeaponData() const;

	void Equip(ACharacter* NewOwner, bool bIsActive);
	void Unequip();

	void SetWeaponVisibility(bool bIsBisible);
	void SetActiveState(bool bIsActive);
	void SetGlidingMeshState(bool bUseGlidingMesh);
};
