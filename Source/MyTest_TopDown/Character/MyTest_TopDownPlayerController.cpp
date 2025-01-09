// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTest_TopDownPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "MyTest_TopDownCharacter.h"
#include "SkillComponent.h"

#include "MyInventoryWidget.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "MyTest_TopDownGameMode.h"

AMyTest_TopDownPlayerController::AMyTest_TopDownPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	SetDestinationKeyAction.Init(nullptr, static_cast<int32>(Key_End));


	// 코드에서 에셋을 찾아서 호출하기 보다는 에디터에서 블루프린트로 접근해서 세팅해주자 [ 그게 더 안전 ]
	/*
	// Set Interact Input
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Interact
	(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_SetDestination_Interact.IA_SetDestination_Interact'"));
	if (IA_Interact.Succeeded())
		SetDestinationInteractAction = IA_Interact.Object;
	
	// Set Tab Input
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Tab
	(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_SetDestination_Tab.IA_SetDestination_Tab'"));
	if (IA_Tab.Succeeded())
		SetDestinationTabAction = IA_Tab.Object;
	
	
	// Count NumberKey
	int32 NumberKey = 0;
	
	// Set Key_Q Input
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Q
	(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_SetDestination_Q.IA_SetDestination_Q'"));
	if (IA_Q.Succeeded())
		SetDestinationKeyAction[NumberKey++] = IA_Q.Object;
	
	// Set Key_W Input
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_W
	(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_SetDestination_W.IA_SetDestination_W'")); 
	if (IA_W.Succeeded())
		SetDestinationKeyAction[NumberKey++] = IA_W.Object;
	
	// Set Key_E Input
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_E
	(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_SetDestination_E.IA_SetDestination_E'"));
	if (IA_E.Succeeded())
		SetDestinationKeyAction[NumberKey++] = IA_E.Object;
	
	// Set Key_R Input
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_R
	(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_SetDestination_R.IA_SetDestination_R'"));
	if (IA_R.Succeeded())
		SetDestinationKeyAction[NumberKey++] = IA_R.Object;
	
	
	// Set Key_V Input
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_V
	(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_SetDestination_V.IA_SetDestination_V'"));
	if (IA_V.Succeeded())
		SetDestinationKeyAction[NumberKey++] = IA_V.Object;
	*/

	/*
	// 인벤토리 UI 만들어주기 .
	static ConstructorHelpers::FClassFinder<UMyInventoryWidget> UI_Inventory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TopDown/UI/Inventory/WBP_MyInventoryWidget.WBP_MyInventoryWidget_C'"));
	if (UI_Inventory.Succeeded())
	{
		Inventory_Class = UI_Inventory.Class;
	
		// Create the widget instance.
		m_InventoryWidget = CreateWidget(GetWorld(), Inventory_Class);
		if (m_InventoryWidget)
		{
			// Add the widget to the viewport so that it becomes visible on the screen.
			//InventoryWidget->AddToViewport();
			m_InventoryWidget->Visibility = ESlateVisibility::Hidden;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Fail Inventory"));
	}
	*/
}

void AMyTest_TopDownPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

}

void AMyTest_TopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events (Left)
		EnhancedInputComponent->BindAction(SetDestinationLClickAction, ETriggerEvent::Started, this, &AMyTest_TopDownPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationLClickAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationLClickAction, ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationLClickAction, ETriggerEvent::Canceled, this, &AMyTest_TopDownPlayerController::OnSetDestinationReleased);

		// Setup mouse input events (Right)
		EnhancedInputComponent->BindAction(SetDestinationRClickAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::ClickRMouse);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AMyTest_TopDownPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AMyTest_TopDownPlayerController::OnTouchReleased);


		// Setup Tab/Interact input events
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_Interact], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Interact);

		// Setup Inventory System Key
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_Tab], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::ClickInventory);
		// 0번 (Tab)키를 일시정지하여도 실행되도록 하는 flag 변수 [ 트리거 변수 체크 항목으로 대체 ]
		//EnhancedInputComponent->GetActionBinding(9).bExecuteWhenPaused = true;

		// Setup Key events
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_Q], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Q);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_W], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_W);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_E], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_E);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_R], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_R);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_V], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_V);

		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_Space], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Space);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_Space], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Release_Space);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_Shift], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Shift);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKey::Key_Shift], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Release_Shift);

	}

	Inventory_Notify.AddUObject(this, &AMyTest_TopDownPlayerController::ClickInventory);

}

void AMyTest_TopDownPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AMyTest_TopDownPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AMyTest_TopDownPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AMyTest_TopDownPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AMyTest_TopDownPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AMyTest_TopDownPlayerController::ClickRMouse()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->Attack();
}

void AMyTest_TopDownPlayerController::ClickInventory()
{
	AMyTest_TopDownGameMode* GameMode = CastChecked<AMyTest_TopDownGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode->GetHUDState() == AMyTest_TopDownGameMode::EState::EInventory) // 인벤토리가 열려 있으면 
	{
		GameMode->ChangeHUDState(AMyTest_TopDownGameMode::EState::EIngame);
		SetPause(false);
		
	}
	else						  // 인벤토리가 닫혀 있으면
	{
		GameMode->ChangeHUDState(AMyTest_TopDownGameMode::EState::EInventory);
		SetPause(true);
		
		// InventoryWidget Update Call 
		UMyInventoryWidget* InventoryWidget = CastChecked<UMyInventoryWidget>(GameMode->GetCurrentWidget());
		if (InventoryWidget)
		{
			InventoryWidget->UpdateWidget();
		}
	}

}

void AMyTest_TopDownPlayerController::Interact()
{
	//UE_LOG(LogTemp, Log, TEXT(" Interact ON "));

	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->Interact();
}

void AMyTest_TopDownPlayerController::Click_Q()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->GetSkillComponent()->Click_Q();
	
}

void AMyTest_TopDownPlayerController::Click_W()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->GetSkillComponent()->Click_W();
}

void AMyTest_TopDownPlayerController::Click_E()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->GetSkillComponent()->Click_E();
}

void AMyTest_TopDownPlayerController::Click_R()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->GetSkillComponent()->Click_R();
}

void AMyTest_TopDownPlayerController::Click_V()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->ChangeCameraView();
}

void AMyTest_TopDownPlayerController::Click_Space()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->Jump();
}

void AMyTest_TopDownPlayerController::Click_Shift()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->Sprint();
}

void AMyTest_TopDownPlayerController::Release_Space()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->StopJumping();
}

void AMyTest_TopDownPlayerController::Release_Shift()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->StopSprint();
}
