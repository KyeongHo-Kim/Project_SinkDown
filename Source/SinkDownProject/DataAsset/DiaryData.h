#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DiaryData.generated.h"

UENUM(BlueprintType)
enum class EDiaryPieceType : uint8
{
	Piece1 UMETA(DisplayName = "First Memory"),
	Piece2 UMETA(DisplayName = "Second Memory"),
	Piece3 UMETA(DisplayName = "Third Memory"),
	Piece4 UMETA(DisplayName = "Fourth Memory"),
	Piece5 UMETA(DisplayName = "Final Memory")
};

USTRUCT(BlueprintType)
struct FDiaryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) UTexture2D* Icon;
};


UCLASS()
class SINKDOWNPROJECT_API UDiaryData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diary") TMap<EDiaryPieceType, FDiaryEntry> DiaryPieces;
};
