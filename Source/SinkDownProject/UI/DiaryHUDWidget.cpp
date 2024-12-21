#include "SinkDownProject/UI/DiaryHUDWidget.h"
#include "SinkDownProject/GameLevels/DiarySubsystem.h"
#include "SinkDownProject/DataAsset/DiaryData.h"
#include "Components/Image.h"

void UDiaryHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize
	InitializeIcons();

	// Register a delegate on the subsystem
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UDiarySubsystem* DiarySubsystem = GameInstance->GetSubsystem<UDiarySubsystem>())
		{
			DiarySubsystem->OnDiaryPieceCollected.AddDynamic(this, &UDiaryHUDWidget::OnDiaryPieceCollected);

			// Show fragments already collected
			for (const EDiaryPieceType& PieceType : DiarySubsystem->GetCollectedPieces())
			{
				UpdateIconVisibility(PieceType);
			}
		}
	}
}

void UDiaryHUDWidget::NativeDestruct()
{
	// Remove the delegate
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UDiarySubsystem* DiarySubsystem = GameInstance->GetSubsystem<UDiarySubsystem>())
		{
			DiarySubsystem->OnDiaryPieceCollected.RemoveDynamic(this, &UDiaryHUDWidget::OnDiaryPieceCollected);
		}
	}

	Super::NativeDestruct();
}

void UDiaryHUDWidget::InitializeIcons()
{
	// Reset all icons
	TArray<UImage*> ICons = { DiaryIcon1, DiaryIcon2, DiaryIcon3, DiaryIcon4, DiaryIcon5 };

	for (UImage* ICon : ICons)
	{
		if (ICon)
		{
			ICon->SetOpacity(0.3f);
		}
	}

	// Set icon texture if DataAsset exists
	if (DiaryDataAsset)
	{
		for (const auto& Pair : DiaryDataAsset->DiaryPieces)
		{
			if (UImage* Icon = GetIconForPieceType(Pair.Key))
			{
				if (Pair.Value.Icon)
				{
					Icon->SetBrushFromTexture(Pair.Value.Icon);
				}
			}
		}
	}
}

void UDiaryHUDWidget::OnDiaryPieceCollected(EDiaryPieceType PieceType)
{
	UpdateIconVisibility(PieceType);
}

void UDiaryHUDWidget::UpdateIconVisibility(EDiaryPieceType PieceType)
{
	// make the collection completion status completely opaque
	if (UImage* Icon = GetIconForPieceType(PieceType))
	{
		Icon->SetOpacity(1.0f); 
	}
}

UImage* UDiaryHUDWidget::GetIconForPieceType(EDiaryPieceType PieceType)
{
	switch (PieceType)
	{
	case EDiaryPieceType::Piece1: return DiaryIcon1;
	case EDiaryPieceType::Piece2: return DiaryIcon2;
	case EDiaryPieceType::Piece3: return DiaryIcon3;
	case EDiaryPieceType::Piece4: return DiaryIcon4;
	case EDiaryPieceType::Piece5: return DiaryIcon5;
	default: return nullptr;
	}
}