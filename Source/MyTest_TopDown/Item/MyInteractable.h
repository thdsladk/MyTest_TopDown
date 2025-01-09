// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractable.generated.h"



UCLASS()
class MYTEST_TOPDOWN_API AMyInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyInteractable();

	FString GetHelpText() { return m_InteractableHelpText; }
	bool GetInCharacter() { return m_bInCharacter; }

	void SwitchVisiable() { m_bVisiable = !m_bVisiable; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnCharacterBeginOverlap(
			UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult
		);

	UFUNCTION()
	virtual void OnCharacterEndOverlap(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	virtual void Init();
	void Interact();
	virtual void Interact_Implementation();
	virtual int32 UsingItem() { return 0; }
	virtual void Replace(FVector Pos);
	virtual void SetHidden(bool bHide);


protected:
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	FString m_InteractableHelpText;


	UPROPERTY(VisibleAnywhere, Category = Interactable)
	class UBoxComponent* m_Trigger;
	UPROPERTY(VisibleAnywhere, Category = HelpTip)
	class UWidgetComponent* m_HelpTextComp;
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* m_MeshComp;


	bool m_bVisiable;
	bool m_bInCharacter;



};
