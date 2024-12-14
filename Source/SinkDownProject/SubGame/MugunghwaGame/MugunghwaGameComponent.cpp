#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaGameComponent.h"
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaTagger.h"
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaWatcher.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaWASDWidget.h"
#include "SinkDownProject/DataAsset/MugunghwaGameEffectsData.h"
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaDoor.h"
#include "SinkDownProject/Items/Rewards/Diary.h"

UMugunghwaGameComponent::UMugunghwaGameComponent()
{
	CountingDuration = 0.0f;
	TurningDuration = 0.5f;
	WASDGameDuration = 3.0f;

	CurrentState = EMugunghwaState::Inactive;
}


void UMugunghwaGameComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMugunghwaGameComponent::StartGame()
{
	if (!TaggerActor || !WatcherActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot start game: Required actors are not set"));
		return;
	}

	if (!TaggerActor->OnMugunghwaInteract.IsBound())
	{
		TaggerActor->OnMugunghwaInteract.AddUObject(this, &UMugunghwaGameComponent::ForceEndGame);
	}

	CurrentState = EMugunghwaState::Counting;

	StartCounting();
}

void UMugunghwaGameComponent::EndGame(bool bSuccess)
{
	GetWorld()->GetTimerManager().ClearTimer(MugunghwaGameStateTimer);

	if (bSuccess)
	{
		CurrentState = EMugunghwaState::Completed;
		if (SoundData && SoundData->CorrectKeyPressSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SoundData->WASDGameSuccessSound);
		}
		StartGame();
	}
	else
	{
		CurrentState = EMugunghwaState::Failed;

		if (SoundData && SoundData->WrongKeyPressSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SoundData->WrongKeyPressSound);
		}

		if (WatcherActor)
		{
			WatcherActor->AttackPlayer();
		}

		FTimerHandle DamageTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, 
			[this]()
			{
				if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
				{
					if (UHealthComponent* HealthComp = PlayerCharacter->FindComponentByClass<UHealthComponent>())
					{
						HealthComp->TakeMaxDamage();
						TaggerActor->OnMugunghwaInteract.RemoveAll(this);
					}
				}
			},
			0.5f,  
			false 
		);

		CurrentState = EMugunghwaState::Inactive;
	}

	// Clean up the UI
	if (WASDWidget)
	{
		WASDWidget->HideWidget();
		GetWorld()->GetTimerManager().ClearTimer(TimerUpdateHandle);
	}
}

void UMugunghwaGameComponent::StartCounting()
{
	// Rotate the predicate to face the wall
	TaggerActor->StartCounting();

	if (SoundData && SoundData->MugunghwaNarration)
	{
		// Get sound duration
		float SoundDuration = SoundData->MugunghwaNarration->GetDuration();

		// Generate random pitch between 1.0 and 1.5
		float PitchMultiplier = FMath::RandRange(1.0f, 1.5f);

		// Adjust counting duration based on pitch
		CountingDuration = SoundDuration / PitchMultiplier;

		// Play sound with random pitch
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			SoundData->MugunghwaNarration,
			1.0f,      // Volume Multiplier
			PitchMultiplier  // Pitch Multiplier
		);
	}

	GetWorld()->GetTimerManager().SetTimer(
		MugunghwaGameStateTimer,
		this,
		&UMugunghwaGameComponent::StartTurning,
		CountingDuration,
		false
	);
}

void UMugunghwaGameComponent::StartTurning()
{
	CurrentState = EMugunghwaState::Turning;

	TaggerActor->StartTurning();

	// Calling StartWASDGame after TurningDuration
	GetWorld()->GetTimerManager().SetTimer(MugunghwaGameStateTimer, this, &UMugunghwaGameComponent::StartWASDGame, TurningDuration, false);
}

void UMugunghwaGameComponent::StartWASDGame()
{
	CurrentState = EMugunghwaState::WASDGame;

	// Generate a WASD key sequence
	CurrentKeySequence.Empty();
	TArray<FKey> PossibleKeys = { EKeys::W, EKeys::A, EKeys::S, EKeys::D };

	int32 SequenceLength = FMath::RandRange(4, 8);

	for (int32 i = 0; i < SequenceLength; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, PossibleKeys.Num() - 1);
		CurrentKeySequence.Add(PossibleKeys[RandomIndex]);
	}
	CurrentKeyIndex = 0;

	// Handling Failure after WASDGameDuration
	GetWorld()->GetTimerManager().SetTimer(MugunghwaGameStateTimer, [this]() { EndGame(false); }, WASDGameDuration, false);

	// Create and display UI widgets
	if (WASDWidgetClass && !WASDWidget)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			WASDWidget = CreateWidget<UMugunghwaWASDWidget>(PC, WASDWidgetClass);
		}
	}

	if (WASDWidget)
	{
		WASDWidget->SetMaxTime(WASDGameDuration);  
		WASDWidget->ShowWidget();
		WASDWidget->UpdateKeySequence(CurrentKeySequence, CurrentKeyIndex);

		// Start updating the timer
		GetWorld()->GetTimerManager().SetTimer(TimerUpdateHandle, this, &UMugunghwaGameComponent::UpdateTimeRemaining, 0.1f, true);
	}

	// Set a failure timer
	GetWorld()->GetTimerManager().SetTimer(MugunghwaGameStateTimer, [this]() { EndGame(false); }, WASDGameDuration, false);
}

void UMugunghwaGameComponent::UpdateTimeRemaining()
{
	if (!WASDWidget) return;

	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(MugunghwaGameStateTimer);
	WASDWidget->UpdateTimeRemaining(RemainingTime);
}


bool UMugunghwaGameComponent::ValidateKeyInput(FKey PressedKey)
{
	if (CurrentState != EMugunghwaState::WASDGame) return false;

	if (PressedKey == CurrentKeySequence[CurrentKeyIndex])
	{
		if (SoundData && SoundData->CorrectKeyPressSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SoundData->CorrectKeyPressSound);
		}

		CurrentKeyIndex++;

		// Update UI
		if (WASDWidget)
		{
			WASDWidget->UpdateKeySequence(CurrentKeySequence, CurrentKeyIndex);
		}

		// If you entered all the keys correctly
		if (CurrentKeyIndex >= CurrentKeySequence.Num())
		{
			EndGame(true);
			return true;
		}
	}
	else
	{
		if (SoundData && SoundData->WrongKeyPressSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SoundData->WrongKeyPressSound);
		}
		EndGame(false);
	}

	return false;
}

void UMugunghwaGameComponent::ForceEndGame()
{
	if (SoundData && SoundData->EndMugunghwaGameSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundData->EndMugunghwaGameSound);
		UGameplayStatics::PlaySound2D(GetWorld(), SoundData->MugunghwaOpenDoorSound);
	}

	AMugunghwaDoor* Door = Cast<AMugunghwaDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), AMugunghwaDoor::StaticClass()));
	if (Door)
	{
		Door->OpenDoor();
	}

	GetWorld()->GetTimerManager().ClearTimer(MugunghwaGameStateTimer);

	// Clean up the UI
	if (WASDWidget)
	{
		WASDWidget->HideWidget();
		GetWorld()->GetTimerManager().ClearTimer(TimerUpdateHandle);
	}

	CurrentState = EMugunghwaState::Inactive;

	if (TaggerActor)
	{
		SpawnDiary();

		TaggerActor->StartPhysicsDestruction();

		FTimerHandle DestroyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DestroyTimerHandle,
			[this]()
			{
				if (TaggerActor)
				{
					TaggerActor->OnMugunghwaInteract.RemoveAll(this);
					TaggerActor->Destroy();
				}
			},
			2.0f, 
			false
		);
	}

}

ADiary* UMugunghwaGameComponent::SpawnDiary()
{
	if (!DiaryClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid parameters passed to SpawnDiary."));
		return nullptr;
	}

	// Get Tagger location and rotation
	FVector TaggerLocation = TaggerActor->GetActorLocation();
	FRotator TaggerRotation = TaggerActor->GetActorRotation();

	// Prepare spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the Diary actor
	ADiary* SpawnedDiary = GetWorld()->SpawnActor<ADiary>(DiaryClass, TaggerLocation, TaggerRotation, SpawnParams);

	if (!SpawnedDiary)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Diary actor."));
	}

	return SpawnedDiary;
}
