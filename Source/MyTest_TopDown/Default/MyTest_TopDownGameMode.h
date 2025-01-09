// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyTest_TopDownGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHUDUpdate, uint8);

UCLASS(minimalapi)
class AMyTest_TopDownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyTest_TopDownGameMode();

	enum EState : uint8
	{
		EIngame,
		EInventory,
		EShop,
		EStatus,
		ESkill,
		EEnd
	};

	UUserWidget* GetCurrentWidget() { return CurrentWidget; }
	uint8 GetHUDState() { return m_State; }
	void ApplyHUDChange();

	UFUNCTION(BlueprintCallable, Category = "HUD_Functions")
	void ChangeHUDState(uint8 State);
	
	bool ApplyHUD(TSubclassOf<UUserWidget> Widget, bool bShowMouse, bool EnableClickEvent);

	FOnHUDUpdate OnHUDUpdate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected ="true"))
	TSubclassOf<UUserWidget> HUD_Class;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> m_InventoryWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> m_ShopWidget;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	UUserWidget* CurrentWidget;

	uint8 m_State = 0;
};



