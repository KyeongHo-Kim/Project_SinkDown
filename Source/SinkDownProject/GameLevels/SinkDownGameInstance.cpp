#include "SinkDownProject/GameLevels/SinkDownGameInstance.h"
#include "SinkDownProject/UI/LoadingScreenWidget.h"
#include "Kismet/GameplayStatics.h"

void USinkDownGameInstance::LoadGameLevel()
{
    LoadedSteps = 0;
    TotalLoadSteps = 4;  // Character, respawn, UI, gameplay content

    bLoadingComplete = false;
    LoadingStartTime = GetWorld()->GetTimeSeconds();

    // Create and display a loading screen
    if (LoadingScreenClass)
    {
        LoadingScreenWidget = CreateWidget<ULoadingScreenWidget>(this, LoadingScreenClass);
        if (!LoadingScreenWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create LoadingScreenWidget"));
            return;
        }

        LoadingScreenWidget->AddToViewport(100);  // Show on top layer
        LoadingScreenWidget->UpdateProgress(0.0f);

        LoadingScreenWidget->PlayLoadingVideo();

        // Start loading each piece of content
        LoadCharacterContent();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadingScreenClass is not set"));
    }
}

void USinkDownGameInstance::LoadCharacterContent()
{
    if (!LoadingScreenWidget) return;

    LoadingScreenWidget->SetLoadingText(FText::FromString(TEXT("Loading Character...")));

    // Paths to character-related assets
    TArray<FSoftObjectPath> CharacterAssets;
    CharacterAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Player/BP_Player")));

    // Asynchronous load request
    for (const FSoftObjectPath& AssetPath : CharacterAssets)
    {
        StreamableManager.RequestAsyncLoad(AssetPath,
            FStreamableDelegate::CreateUObject(this, &USinkDownGameInstance::OnAssetLoaded));
    }
}

void USinkDownGameInstance::LoadRespawnContent()
{
    LoadingScreenWidget->SetLoadingText(FText::FromString(TEXT("Loading Respawn System...")));

    // Assets related to the respawn system
    TArray<FSoftObjectPath> RespawnAssets;
    RespawnAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Artefacts/BP_RespawnPoint_Stand")));
    RespawnAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Artefacts/BP_RespawnPoint_Down")));

    for (const FSoftObjectPath& AssetPath : RespawnAssets)
    {
        StreamableManager.RequestAsyncLoad(AssetPath,
            FStreamableDelegate::CreateUObject(this, &USinkDownGameInstance::OnAssetLoaded));
    }
}

void USinkDownGameInstance::LoadUIContent()
{
    LoadingScreenWidget->SetLoadingText(FText::FromString(TEXT("Loading UI...")));

    // UI-related assets
    TArray<FSoftObjectPath> UIAssets;
    // Add paths to UI assets such as HUDs...

    for (const FSoftObjectPath& AssetPath : UIAssets)
    {
        StreamableManager.RequestAsyncLoad(AssetPath,
            FStreamableDelegate::CreateUObject(this, &USinkDownGameInstance::OnAssetLoaded));
    }
}

void USinkDownGameInstance::LoadGameplayContent()
{
    LoadingScreenWidget->SetLoadingText(FText::FromString(TEXT("Loading Gameplay Elements...")));

    // Gameplay-related assets
    TArray<FSoftObjectPath> GameplayAssets;

    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_plast_001")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_plast_002")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_plast_005")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/SM_toys_plast_005_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/cobwebs_cobweb_001")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/cobwebs_cobweb_003")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/cobwebs_cobweb_004")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/cobwebs_cobweb_006")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/cobwebs_cobweb_007")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/SteppingStonesGame/BP_FakeGround_Big")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/SteppingStonesGame/BP_TrueGround_Big")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/SteppingStonesGame/BP_SteppingStoneActivator")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/NK_cupboard")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_kids_a4_000")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_Maze_3")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_Shelf01_Line001")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_Shelf01_SM_Shelf01")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_toy")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_toys_bear")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_toys_rabbit")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_toys_wood_cubes_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_book_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_book_3")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_book_4")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_book_5")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_book_6")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_book_7")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_book_8")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_box")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_fabr_toys_001_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_kids_a4_000")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ModernDesk/BackgroundProps/StaticMeshs/SM_Lamp01")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_wood_cubes_002")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/MetalRuler")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_Laptop")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_bag")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_chair_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_chair_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_chair_3")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_cube_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_cup")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ModernDesk/BackgroundProps/StaticMeshs/SM_Eraser01")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_glass_pencils")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_kids_a4_002")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_pencils")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_table")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Artefacts/BP_JumpPad")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Items/BP_AcquirableItem_Umbrella")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Items/BP_AcquirableItem_Watergun")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_bed_3")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_commode")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_commode_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_commode_cloth_3")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_Maze")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_Maze_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_Maze_4")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_toys_wood_cubes_3")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Nkasset/Nk_toys_wood_cubes_9")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Megascans/3D_Assets/Cactus_Pot_uenkeewfa/S_Cactus_Pot_uenkeewfa_lod3_Var1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_blanket_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_carpet")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_clothes_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_clothes_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_clothes_3")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_fabr_toys_001")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_fabr_toys_002_hor")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_fabr_toys_003")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_fabr_toys_004")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_frame_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_frame_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_frame_3")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_frame_4")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_jalousi_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ModernDesk/BackgroundProps/StaticMeshs/SM_Frame02")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_kids_carpet")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_lamp")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_pillow_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_pillow_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ModernDesk/BackgroundProps/StaticMeshs/SM_Pot01")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_radiator")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ModernDesk/BackgroundProps/StaticMeshs/SM_Socket01")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_soft_toys_005")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_soft_toys_006")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_toys_ball_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_toys_bear")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_toys_car_1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_toys_car_2")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_cubes_001")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_cubes_003")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_toys_dinosaur")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_dolls_hor_005")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_toys_horse")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_plast_003")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_plast_004")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_toys_rabbit")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/ToysSet/Meshes/SM_toys_wood_cubes_001")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_troley")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/ToysAsset/Map1/Meshes/SM_trumpet")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Human1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Animal_S_PInk")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Animal_S_White")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Human_light_Big")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Animal_Blue")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Animal_pink_pig")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Human_light1")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_RangeEnemy_Pink")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Animal_Brown")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Enemy/BP_MeleeEnemy_Animal_White_pig")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Artefacts/BP_SurveillanceLight")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Artefacts/BP_ToyAirplane")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/MugunghwaGame/BP_MugunghwaActivator")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/MugunghwaGame/BP_MugunghwaDoor")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/MugunghwaGame/BP_MugunghwaObstacle")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/MugunghwaGame/BP_MugunghwaSurveillanceLight")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/MugunghwaGame/BP_MugunghwaTagger")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/MugunghwaGame/BP_MugunghwaWatcher")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/RollingBallGame/BP_RollingBallActivator")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/RollingBallGame/BP_RollingBallEndPoint")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/SubGame/RollingBallGame/BP_RollingBallEndPoint")));

    // Sound
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/Sound/BaseBGM/Base/SC_InGame_BaseBGM")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/Sound/BaseBGM/HiPPoZoneBGM/SC_HippoZomBGM")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/Sound/BaseBGM/MemoriesZone/Memories")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/Sound/BaseBGM/MugungMazeBGMZone/SC_MugungHwa_BGM")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/Sound/BaseBGM/RadiatorZoneBGM/RadiatorZoneBgm")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Asset/Sound/BaseBGM/SteppingZoneBGM/SteppingStonesGameBGM")));

    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Player/ABP_Player")));

    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Effects/Sounds/BP_GameSoundManager")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Effects/Sounds/INGame/BP_HippoZoneTrigger")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Effects/Sounds/INGame/BP_MemoriesZoneTrigger")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Effects/Sounds/INGame/BP_MugunghwaGameZoneTrigger")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Effects/Sounds/INGame/BP_MugunghwaMazeZoneTrigger")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Effects/Sounds/INGame/BP_RadiatorZoneTrigger")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Effects/Sounds/INGame/BP_SteppingStonesGameBGMZoneTrigger")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/BluePrints/Effects/Sounds/INGame/BP_MyhomeZoneTrigger")));

    //Animation
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Animations/Player/Combat/Attack/Melee/MeleeAttackMontage")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Animations/Player/Combat/Attack/Melee/AM_Skill_Parry")));
    GameplayAssets.Add(FSoftObjectPath(TEXT("/Game/Animations/Player/Combat/AM_PlayerDeath")));



   

    for (const FSoftObjectPath& AssetPath : GameplayAssets)
    {
        StreamableManager.RequestAsyncLoad(AssetPath,
            FStreamableDelegate::CreateUObject(this, &USinkDownGameInstance::OnAssetLoaded));
    }
}

void USinkDownGameInstance::OnAssetLoaded()
{
    LoadedSteps++;
    float Progress = static_cast<float>(LoadedSteps) / TotalLoadSteps;

    if (LoadingScreenWidget)
    {
        LoadingScreenWidget->UpdateProgress(Progress);
    }

    // Load Next Step
    if (LoadedSteps == 1)
    {
        LoadRespawnContent();
    }
    else if (LoadedSteps == 2)
    {
        LoadUIContent();
    }
    else if (LoadedSteps == 3)
    {
        LoadGameplayContent();
    }
    else if (LoadedSteps >= TotalLoadSteps)
    {
        /*FinishLoading();*/
        bLoadingComplete = true;
        CheckAndFinishLoading();
    }
}

void USinkDownGameInstance::CheckAndFinishLoading()
{
    if (!bLoadingComplete) return;

    float CurrentTime = GetWorld()->GetTimeSeconds();
    float ElapsedTime = CurrentTime - LoadingStartTime;

    if (ElapsedTime < MinimumLoadingScreenTime)
    {
        // Set a timer if the minimum time hasn't passed yet
        FTimerHandle WaitHandle;
        float RemainingTime = MinimumLoadingScreenTime - ElapsedTime;
        GetWorld()->GetTimerManager().SetTimer(WaitHandle, this, &USinkDownGameInstance::FinishLoading, RemainingTime, false);
    }
    else
    {
        // Complete as soon as the minimum time has passed
        FinishLoading();
    }
}

void USinkDownGameInstance::FinishLoading()
{

    APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
    if (CameraManager)
    {
        CameraManager->StartCameraFade(0.0f, 1.0f, 1.0f, FLinearColor::Black, true, true);
    }

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            // Stop video on loading screen
            if (LoadingScreenWidget)
            {
                LoadingScreenWidget->StopLoadingVideo();
            }

            // Switch to the game level
            UGameplayStatics::OpenLevel(this, TEXT("GameLevel"));

            // Remove the loading screen
            if (LoadingScreenWidget)
            {
                LoadingScreenWidget->RemoveFromParent();
                LoadingScreenWidget = nullptr;
            }

            // Fade in at the new level
            FTimerHandle FadeInHandle;
            GetWorld()->GetTimerManager().SetTimer(FadeInHandle, [this]()
                {
                    if (APlayerCameraManager* NewCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
                    {
                        NewCameraManager->StartCameraFade(1.0f, 0.0f, 1.0f, FLinearColor::Black, false);
                    }
                }, 0.1f, false);
        }, 1.0f, false);
}