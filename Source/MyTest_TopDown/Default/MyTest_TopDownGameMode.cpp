// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTest_TopDownGameMode.h"
#include "MyTest_TopDownPlayerController.h"
#include "MyTest_TopDownCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "MyHUD.h"
#include "MyInventoryWidget.h"


AMyTest_TopDownGameMode::AMyTest_TopDownGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyTest_TopDownPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}



	static ConstructorHelpers::FClassFinder<UMyHUD> UI_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TopDown/UI/BP_MyHUD.BP_MyHUD_C'"));
	if (UI_HUD.Succeeded())
	{
		HUD_Class = UI_HUD.Class;

		// Create the widget instance.
		CurrentWidget = CreateWidget(GetWorld(), HUD_Class);
		if (CurrentWidget)
		{
			// Add the widget to the viewport so that it becomes visible on the screen.
			CurrentWidget->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Fail HUD"));
	}

	static ConstructorHelpers::FClassFinder<UMyInventoryWidget> Inventory_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TopDown/UI/Inventory/WBP_MyInventoryWidget.WBP_MyInventoryWidget_C'"));
	if (Inventory_HUD.Succeeded())
	{
		m_InventoryWidget = Inventory_HUD.Class;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Fail Inventory HUD"));
	}

}

void AMyTest_TopDownGameMode::ApplyHUDChange()
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
	}

	switch (m_State)
	{
	case EState::EIngame:
	{
		ApplyHUD(HUD_Class, true, true);
		break;
	}
	case EState::EInventory :
	{
		ApplyHUD(m_InventoryWidget, true, true);
		break;
	}
	case EState::EShop:
	{
		ApplyHUD(m_ShopWidget, true, true);
		break;
	}
	case EState::EStatus:
	{
		ApplyHUD(HUD_Class, true, true);
		break;
	}
	case EState::ESkill:
	{
		ApplyHUD(HUD_Class, true, true);
		break;
	}
	default :
		break;
	}
}

void AMyTest_TopDownGameMode::ChangeHUDState(uint8 State)
{
	m_State = State;
	ApplyHUDChange();
}

bool AMyTest_TopDownGameMode::ApplyHUD(TSubclassOf<UUserWidget> Widget, bool bShowMouse, bool EnableClickEvent)
{
	//AMyTest_TopDownCharacter* Character = Cast<AMyTest_TopDownCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AMyTest_TopDownPlayerController* Controller = Cast<AMyTest_TopDownPlayerController>(GetWorld()->GetFirstPlayerController());
	
	if (Controller != nullptr)
	{

		Controller->bShowMouseCursor = bShowMouse;
		Controller->bEnableClickEvents = EnableClickEvent;

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);
		
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			// Widget이 변경 될때마다 알리자 . 
			OnHUDUpdate.Broadcast(m_State);

			return true;
		}
	}
	
	
	
	return false;
}
