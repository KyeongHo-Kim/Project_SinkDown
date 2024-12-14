#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{	
	MeleeWeapon UMETA(DisplayName = "MeleeEnemy"),
	RangeWeapon UMETA(DisplayName = "RangeEnemy"),
	None UMETA(DisplayName = "None")
};

class USkeletalMesh;

UCLASS()
class SINKDOWNPROJECT_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Gliding") USkeletalMesh* GlidingSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") FName ActiveSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") FName InactiveSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") float WeaponRange;
};
