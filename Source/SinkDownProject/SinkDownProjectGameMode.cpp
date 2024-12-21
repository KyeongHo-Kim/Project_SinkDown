#include "SinkDownProjectGameMode.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "SinkDownProject/Artefacts/RespawnPoint.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownProject/Animations/PlayerAnimInstance.h"
#include "SinkDownProject/UI/DiaryHUDWidget.h"

ASinkDownProjectGameMode::ASinkDownProjectGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrints/Player/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	RespawnDelay = 3.0f;
}

void ASinkDownProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find all Respawn Point in the Level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnPoint::StaticClass(), reinterpret_cast<TArray<AActor*>&>(RespawnPoints));

	// Switch to game input mode
	if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->SetShowMouseCursor(false);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);

		// Create and display the Diary HUD Widget
		if (DiaryHUDWidgetClass)
		{
			DiaryHUDWidget = CreateWidget<UDiaryHUDWidget>(PC, DiaryHUDWidgetClass);
			if (DiaryHUDWidget)
			{
				DiaryHUDWidget->AddToViewport();
			}
		}
	}
}

void ASinkDownProjectGameMode::RespawnPlayer(ASinkDownProjectCharacter* DeadPlayer)
{
	if (!DeadPlayer) return;

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;
	RespawnDelegate.BindUObject(this, &ASinkDownProjectGameMode::HandleRespawn, DeadPlayer);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
}

ARespawnPoint* ASinkDownProjectGameMode::FindClosestRespawnPoint(const FVector& Location)
{
	ARespawnPoint* ClosestPoint = nullptr;
	float ClosestDistance = MAX_FLT;

	for (ARespawnPoint* Point : RespawnPoints)
	{
		if (!Point) continue;

		float Distance = FVector::Distance(Location, Point->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestPoint = Point;
		}
	}

	return ClosestPoint;
}

void ASinkDownProjectGameMode::HandleRespawn(ASinkDownProjectCharacter* Player)
{
	if (!Player) return;

	ARespawnPoint* RespawnPoint = FindClosestRespawnPoint(Player->GetActorLocation());
	if (!RespawnPoint) return;

	if (UHealthComponent* HealthComp = Player->GetHealthComponent())
	{
		HealthComp->Reset();
	}

	// Reset animation state
	if (UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(Player->GetMesh()->GetAnimInstance()))
	{
		AnimInstance->ResetDeathState();
	}

	if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
	{
		Player->EnableInput(PC);
	}

	// Teleport to respawn point
	Player->SetActorLocation(RespawnPoint->GetActorLocation() + FVector(50.0f,120.0f,0.0f));
	Player->SetActorRotation(RespawnPoint->GetActorRotation());
}
