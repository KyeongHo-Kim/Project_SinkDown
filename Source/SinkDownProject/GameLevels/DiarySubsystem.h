#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SinkDownProject/DataAsset/DiaryData.h"
#include "DiarySubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiaryPieceCollected, EDiaryPieceType, PieceType);

UCLASS()
class SINKDOWNPROJECT_API UDiarySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Initialize and clean up
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY() TArray<EDiaryPieceType> CollectedPieces;

public:
	// Collect diary fragments
	UFUNCTION(BlueprintCallable, Category = "Diary") void CollectDiaryPiece(EDiaryPieceType PieceType);

	// Check if diary fragments are collected
	UFUNCTION(BlueprintCallable, Category = "Diary") bool HasCollectedPiece(EDiaryPieceType PieceType) const;

	// Get all collected diary fragments
	UFUNCTION(BlueprintPure, Category = "Diary") const TArray<EDiaryPieceType>& GetCollectedPieces() const { return CollectedPieces; }

	// Delegate called when collecting diary fragments
	UPROPERTY(BlueprintAssignable, Category = "Diary") FOnDiaryPieceCollected OnDiaryPieceCollected;
};
