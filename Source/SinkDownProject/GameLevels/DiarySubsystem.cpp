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

        CheckAllPiecesCollected();
    }
}

// Check if PieceType exists in the array
bool UDiarySubsystem::HasCollectedPiece(EDiaryPieceType PieceType) const
{
    return CollectedPieces.Contains(PieceType);
}

// Verify that all fragments (5) have been collected
bool UDiarySubsystem::HasCollectedAllPieces() const
{
    return CollectedPieces.Num() >= 5;  
}

void UDiarySubsystem::CheckAllPiecesCollected()
{
    if (HasCollectedAllPieces())
    {
        OnAllDiaryPiecesCollected.Broadcast();
    }
}
