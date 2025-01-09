// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyTest_TopDownPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
DECLARE_MULTICAST_DELEGATE(FOnInvetory);



UCLASS()
class AMyTest_TopDownPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	enum EKey : int8
	{
		Key_Q,
		Key_W,
		Key_E,
		Key_R,
		Key_V,
		Key_Space,
		Key_Shift,
		Key_Interact,
		Key_Tab,
		Key_End

	};
public:
	AMyTest_TopDownPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Left Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationLClickAction;

	/** Right Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationRClickAction;

	/** Touch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationTouchAction;

	/**  Key Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<class UInputAction*> SetDestinationKeyAction;

public:
	FOnInvetory Inventory_Notify;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	void ClickRMouse();

	void ClickInventory();

	void Interact();

	void Click_Q();
	void Click_W();
	void Click_E();
	void Click_R();
	void Click_V();
	void Click_Space();
	void Click_Shift();
	void Release_Space();
	void Release_Shift();



private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	int8 m_UsedKeyNum = Key_End;

protected:


};


