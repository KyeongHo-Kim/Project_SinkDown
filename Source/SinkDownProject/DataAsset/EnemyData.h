#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	MeleeEnemy UMETA(DisplayName = "MeleeEnemy"),
	RangeEnemy UMETA(DisplayName = "RangeEnemy"),
    BossEnemy UMETA(DisplayName = "BossEnemy"),
    Player UMETA(DisplayName = "Player")
};

UCLASS()
class SINKDOWNPROJECT_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    FName EnemyName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    EEnemyType EnemyType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    float DetectedRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    float AttackRange;


};
