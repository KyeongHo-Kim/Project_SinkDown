#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "SinkDownProject/DataAsset/DiaryData.h"
#include "Diary.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class SINKDOWNPROJECT_API ADiary : public AInteractiveObjectBase
{
	GENERATED_BODY()

public:
	ADiary();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnInteract() override;

private:
	UPROPERTY(EditAnywhere) UStaticMeshComponent* DiaryMesh;

	UPROPERTY(EditAnywhere, Category = "Animation") float SpawnDuration;

	UPROPERTY(EditAnywhere, Category = "VFX") UNiagaraSystem* InteractionEffect;

	UPROPERTY(EditAnywhere, Category = "Animation") float DestroyDuration;

	float CurrentSpawnTime;
	float CurrentDestroyTime;
	bool bIsSpawning;
	bool bIsDestroying;

	void StartDestruction();


	// UI
	UPROPERTY(EditAnywhere, Category = "Diary") EDiaryPieceType PieceType;
	UPROPERTY(EditAnywhere, Category = "Diary") UDiaryData* DiaryData;
};
