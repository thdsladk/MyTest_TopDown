// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_RedApple.generated.h"


UCLASS()
class MYTEST_TOPDOWN_API AItem_RedApple : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_RedApple();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class UStaticMeshComponent* RedApple;

private:
	UFUNCTION()
	void OnCharacterOverlap(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult
	);
};
