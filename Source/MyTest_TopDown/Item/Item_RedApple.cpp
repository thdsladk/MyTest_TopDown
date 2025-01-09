// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_RedApple.h"
#include "Components/BoxComponent.h"
#include "MyTest_TopDownCharacter.h"


// Sets default values
AItem_RedApple::AItem_RedApple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	RedApple = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("REDAPPLE"));

	RootComponent = Trigger;
	RedApple->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(30.f, 30.f, 30.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_REDAPPLE(TEXT("/Script/Engine.StaticMesh'/Game/MS_Fruits/3D_Asset/02_Yellow_Red_Nectarine_tersdibra/tersdibra_LOD0.tersdibra_LOD0'"));
	if (SM_REDAPPLE.Succeeded())
	{
		RedApple->SetStaticMesh(SM_REDAPPLE.Object);
	}
	RedApple->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	RedApple->SetCollisionProfileName(TEXT("ItemBox"));

}

// Called when the game starts or when spawned
void AItem_RedApple::BeginPlay()
{
	Super::BeginPlay();

}

void AItem_RedApple::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItem_RedApple::OnCharacterOverlap);

}

void AItem_RedApple::OnCharacterOverlap(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	// TO DO  
	AMyTest_TopDownCharacter* MyCharacter = Cast<AMyTest_TopDownCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{

	}


}


