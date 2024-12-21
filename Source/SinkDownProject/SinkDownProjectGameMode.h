#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SinkDownProjectGameMode.generated.h"

class ARespawnPoint;
class ASinkDownProjectCharacter;
class UDiaryHUDWidget;

UCLASS(minimalapi)
class ASinkDownProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASinkDownProjectGameMode();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY() TArray<ARespawnPoint*> RespawnPoints;

	void HandleRespawn(ASinkDownProjectCharacter* Player);


	UPROPERTY() UDiaryHUDWidget* DiaryHUDWidget; 
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UDiaryHUDWidget> DiaryHUDWidgetClass;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "RespawnDelay") float RespawnDelay;

	ARespawnPoint* FindClosestRespawnPoint(const FVector& Location);

public:
	void RespawnPlayer(ASinkDownProjectCharacter* DeadPlayer);
	
};



