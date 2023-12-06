// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Coursework/CourseworkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "InventoryModule.generated.h"

// Struct for item information
USTRUCT(BlueprintType) struct FSlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Amount;
};

// Modular class that adds an inventory to an actor
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable)
class COURSEWORK_API UInventoryModule : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryModule();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InteractableRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor *ViewedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 InventoryLimit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerCameraManager* Camera;
	
	// Functions
	UFUNCTION(BlueprintCallable)
		AActor* LookRaycast(const AActor *CharacterReference);

	UFUNCTION(BlueprintCallable)
		bool PickUpItem(const AActor *CharacterReference);

	UFUNCTION(BlueprintCallable)
		void AddToInventory(const FString Name, const int32 Count);

	UFUNCTION(BlueprintCallable)
		bool RemoveFromInventory(const int32 AmountToRemove, const FString ItemNameToRemove, int32 &AmountRemoved);

	UFUNCTION(BlueprintCallable)
		void SetUIControls(const int32 ViewportWidth, const int32 ViewportHeight);
	
	UFUNCTION(BlueprintCallable)
	void SetGameplayControls();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FSlot> Collection;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
