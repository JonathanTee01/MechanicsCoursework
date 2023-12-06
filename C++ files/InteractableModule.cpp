// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableModule.h"

// Sets default values for this component's properties
UInteractableModule::UInteractableModule()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Name = "No Name Given!";
	Amount = 0;
	IsContainer = false;
	// ...
}


// Called when the game starts
void UInteractableModule::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractableModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInteractableModule::InteractWith()
{
	// If it's not a container destroy the actor
	if (IsContainer == false)
	{
		GetOwner()->Destroy();

		return true;
	}

	// Returning false means it's a container
	// This flags to the blueprint to render the UI component
	return false;	
}

// Used for debug
FString UInteractableModule::LookAt(const AActor* CharacterReference)
{
	// Just returns the items name
	return Name;
}

