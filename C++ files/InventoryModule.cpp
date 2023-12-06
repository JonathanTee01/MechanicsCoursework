// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryModule.h"

#include "InteractableModule.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Blueprint/UserWidget.h"
#include "Tasks/GameplayTask_SpawnActor.h"

// Sets default values for this component's properties
UInventoryModule::UInventoryModule()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryModule::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...	
}


// Function for finding if/what object is being looked at
AActor* UInventoryModule::LookRaycast(const AActor* CharacterReference)
{
	// Save the camera manager for future use
	Camera = UGameplayStatics::GetPlayerCameraManager(this, 0);

	// Set values used for the ray cast
	FVector ForwardVector = Camera->GetActorForwardVector();
	FVector StartPoint = Camera->GetCameraLocation();
	FVector EndPoint = StartPoint + (ForwardVector * InteractableRange);

	// Input parameters
	FCollisionQueryParams Parameters;

	// Output from raycast
	FHitResult OutHit;

	// If the ray hits an interactable actor
	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartPoint, EndPoint, ECC_GameTraceChannel2, Parameters))
	{
		// If the hit actor is a new one
		if (OutHit.GetActor() != ViewedActor)
		{
			// Update the actor variable
			ViewedActor = OutHit.GetActor();
		}
	}
	else
	{
		// Otherwise we're not looking at anything
		ViewedActor = nullptr;
	}

	return ViewedActor;
}

void UInventoryModule::AddToInventory(const FString Name, const int32 Count)
{
	// New item data for what we're going to add
	FSlot newItem;
	newItem.ItemName = Name;

	// If the actor is already present in the map
	if (Collection.Contains(Name))
	{
		// Get the old item entry
		FSlot* oldItem = Collection.Find(Name);

		// Add the old amount to the new amount
		newItem.Amount = Count + oldItem->Amount;
	}
	// Otherwise it's a new entry
	else
	{
		newItem.Amount = Count;
	}
	
	// Overwrite any present data or add a new entry 
	Collection.Add(Name, newItem);
}

bool UInventoryModule::PickUpItem(const AActor* CharacterReference)
{
	// If an actor isn't being looked at
	if (!ViewedActor)
	{
		// Return to save resources
		return false;
	}	
	
	// Cast the viewed actors component to an interactableModule
	UInteractableModule* interactable = dynamic_cast<UInteractableModule*>(ViewedActor->GetComponentByClass(UInteractableModule::StaticClass()));
	
	// Returning true means it was a pick up
	if (interactable->InteractWith())
	{
		// Pass the information to the add function
		AddToInventory(interactable->Name, interactable->Amount);
		return true;
	}

	return false;
}

bool UInventoryModule::RemoveFromInventory(const int32 AmountToRemove, const FString ItemNameToRemove, int32 &AmountRemoved)
{
	int32 CurrentAmount = Collection.Find(ItemNameToRemove)->Amount;
	
	// If trying to remove more items than are owned
	if (CurrentAmount <= AmountToRemove)
	{
		// Set the amount to remove to the current amount owned
		// This means we'll remove as many as possible
		AmountRemoved = CurrentAmount;
	}
	else
	{
		// Otherwise we're removing the specified amount
		AmountRemoved = AmountToRemove;
	}

	// If removing all the items owned
	if (CurrentAmount == AmountRemoved)
	{
		// Remove the item
		Collection.Remove(ItemNameToRemove);
	}
	// Otherwise we're removing just a portion of the owned items
	else
	{
		// Make new data
		FSlot newValues;
		newValues.ItemName = ItemNameToRemove;
		newValues.Amount = CurrentAmount - AmountRemoved;

		// Overwrite the old data
		Collection.Add(ItemNameToRemove, newValues);
	}
	
	return true;
}


void UInventoryModule::SetUIControls(const int32 ViewportWidth, const int32 ViewportHeight)
{
	// Centre and show the mouse cursor
	Camera->GetOwningPlayerController()->SetShowMouseCursor(true);
	Camera->GetOwningPlayerController()->SetMouseLocation(ViewportWidth/2, ViewportHeight/2);

	// Disable keyboard input since we don't want to run around while the inventory is open
	UGameplayStatics::GetPlayerCharacter(Camera, 0)->DisableInput(Camera->GetOwningPlayerController());
}

void UInventoryModule::SetGameplayControls()
{
	// Re-enable the player input and show the cursor
	Camera->GetOwningPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::GetPlayerCharacter(Camera, 0)->EnableInput(Camera->GetOwningPlayerController());
}
