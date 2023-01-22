// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay()
// When the game starts
{
    Super::BeginPlay();
    srand(time(NULL));
    InitWordsList();
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input)
// When the player hits enter
{
    if (bGameOver)
    {
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome!"));

    HiddenWord = PickRandomWord(AvailableWords);
    if (DEBUG)
    {
        PrintLine(TEXT("DEBUG: hidden word %s"), *HiddenWord);    
    }
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(
        TEXT("Try to guess the %i letters isogram word."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), HiddenWord.Len());
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    ClearScreen();
    PrintLine(TEXT("You lost!"));
    PrintLine(TEXT("The word was: %s!"), *HiddenWord);
    PrintLine(TEXT("Press enter to play again"));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index=0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison=Index+1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

void UBullCowCartridge::InitWordsList()
{
    // Initializes the Wordlist
    const FString HiddenWordsListPath = TEXT("WordLists/HiddenWordsList.txt");
    const FString WordListPath = FPaths::ProjectContentDir() / HiddenWordsListPath;
    FFileHelper::LoadFileToStringArray(AvailableWords, *WordListPath);
    if (DEBUG)
    {
        PrintLine(TEXT("DEBUG: All the words: %i"), AvailableWords.Num());
    }
}

FString UBullCowCartridge::PickRandomWord(const TArray<FString> WordsList)
{
    int32 WordLength = FMath::RandRange(2, 7);
    TArray<FString> ValidWords;
    for (FString Word : AvailableWords)
    {
        if (Word.Len() == WordLength && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    PrintLine(
        TEXT("DEBUG: The words with %i letters are: %i"),
        WordLength, ValidWords.Num());
    int32 RandomIndex = FMath::RandRange(0, ValidWords.Num() - 1);
    return ValidWords[RandomIndex];
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) const
{
    FBullCowCount Score;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Score.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Score.Cows++;
                break; 
            }
        }
    }
    return Score;
}   

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        bGameOver = true;
        ClearScreen();
        PrintLine(TEXT("Correct! You won!"));
        PrintLine(TEXT("Press enter to play again"));
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Length doesn't match."));
        PrintLine(TEXT("The word must be %i letters long."), HiddenWord.Len());
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("The word is not a isogram. Guess again:"));
        return;
    }

    --Lives;
    if (Lives <= 0)
    {
        EndGame();
        return;
    }
    FBullCowCount Score = GetBullsCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Lives left: %i"), Lives);
}
