// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ClosedDoorYaw = GetOwner()->GetActorRotation().Yaw;
	OpenDoorAngle += ClosedDoorYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate not set on %s"), *GetOwner()->GetName());
	}
	
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor(float DeltaTime) const
{
	FRotator CurrentDoorRotation = GetOwner()->GetActorRotation();
	if (CurrentDoorRotation.Yaw == OpenDoorAngle)
	{
		return;
	}
	CurrentDoorRotation.Yaw = FMath::FInterpTo(CurrentDoorRotation.Yaw, OpenDoorAngle, DeltaTime, DoorClosingSpeed);
	//UE_LOG(LogTemp, Waraning, TEXT("Door Yaw: %.2f"), CurrentDoorRotation.Yaw);
	GetOwner()->SetActorRotation(CurrentDoorRotation);
	if (CurrentDoorRotation.Yaw == OpenDoorAngle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Opened door: %s"), *GetOwner()->GetName());
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) const
{
	FRotator CurrentDoorRotation = GetOwner()->GetActorRotation();
	if (CurrentDoorRotation.Yaw == ClosedDoorYaw)
	{
		return;
	}
	CurrentDoorRotation.Yaw = FMath::FInterpTo(CurrentDoorRotation.Yaw, ClosedDoorYaw, DeltaTime, DoorClosingSpeed);
	GetOwner()->SetActorRotation(CurrentDoorRotation);
	if (CurrentDoorRotation.Yaw == ClosedDoorYaw)
	{
		UE_LOG(LogTemp, Warning, TEXT("Closed door: %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float Now = GetWorld()->GetTimeSeconds();
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpenedSeconds = Now;
		return;
	}
	
	if (DoorLastOpenedSeconds + DoorCloseDelaySeconds <= Now)
	{
		CloseDoor(DeltaTime);
	}
}
