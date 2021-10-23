// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	bool IsIsogram(const FString& Word) const;
	void ProcessGuess(const FString& Guess);
	FBullCowCount GetBullsCows(const FString& Guess) const;

	private:
	void InitWordsList();
	FString PickRandomWord(const TArray<FString> WordsList);

	TArray<FString> AvailableWords;
	const bool DEBUG = true;

	FString HiddenWord;
	int32 Lives;
	bool bGameOver;
};
