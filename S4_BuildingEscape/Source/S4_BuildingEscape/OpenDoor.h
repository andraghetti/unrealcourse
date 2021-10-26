// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class S4_BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float ClosedDoorYaw;
	UPROPERTY(EditAnywhere) bool OpenDoorTowardsInside = true;
	UPROPERTY(EditAnywhere) float OpenDoorAngle = 90.f;
	UPROPERTY(EditAnywhere) float DoorClosingSpeed = 2.f;
	UPROPERTY(EditAnywhere) float DoorCloseDelaySeconds = 2.f;
	UPROPERTY(EditAnywhere) ATriggerVolume* PressurePlate;
	UPROPERTY(EditAnywhere) AActor* ActorThatOpens;

	void OpenDoor(float DeltaTime) const;
	void CloseDoor(float DeltaTime) const;

	float DoorLastOpenedSeconds = 0.f;
};
