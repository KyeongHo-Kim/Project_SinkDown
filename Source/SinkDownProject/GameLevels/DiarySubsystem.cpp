#include "SinkDownProject/GameLevels/DiarySubsystem.h"

void UDiarySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDiarySubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDiarySubsystem::CollectDiaryPiece(EDiaryPieceType PieceType)
{
    // Add only if not already collected fragments
    if (!HasCollectedPiece(PieceType))
    {
        CollectedPieces.Add(PieceType);
        OnDiaryPieceCollected.Broadcast(PieceType);

        UE_LOG(LogTemp, Warning, TEXT("Collected Diary Piece: %d"), static_cast<int32>(PieceType));
    }
}

// Check if PieceType exists in the array
bool UDiarySubsystem::HasCollectedPiece(EDiaryPieceType PieceType) const
{
    return CollectedPieces.Contains(PieceType);
}