// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintRegexBPLibrary.h"
#include "BlueprintRegex.h"
#include "Internationalization/Regex.h"

UBlueprintRegexBPLibrary::UBlueprintRegexBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

int UBlueprintRegexBPLibrary::Count(FString Input, FString Pattern)
{
	FRegexMatcher Matcher(FRegexPattern(Pattern), Input);

	int32 Result = 0;
	while (Matcher.FindNext())
	{
		++Result;
	}

	return Result;
}

bool UBlueprintRegexBPLibrary::IsMatch(FString Input, FString Pattern)
{
	return FRegexMatcher(FRegexPattern(Pattern), Input).FindNext();
}

TArray<FRegexMatch> UBlueprintRegexBPLibrary::Matches(FString Input, FString Pattern)
{
	FRegexMatcher Matcher(FRegexPattern(Pattern), Input);

	TArray<FRegexMatch> Result;

	while (Matcher.FindNext())
	{
		FRegexMatch Match;
		Match.Index = Matcher.GetMatchBeginning();
		Match.Lenght = Matcher.GetMatchEnding() - Match.Index;
		Match.Value = Input.Mid(Match.Index, Match.Lenght);

		Result.Add(Match);
	}

	return Result;
}

TArray<FString> UBlueprintRegexBPLibrary::StringMatches(FString Input, FString Pattern)
{
	FRegexMatcher Matcher(FRegexPattern(Pattern), Input);

	TArray<FString> Result;

	while (Matcher.FindNext())
	{
		Result.Add(Input.Mid(Matcher.GetMatchBeginning(), Matcher.GetMatchEnding() - Matcher.GetMatchBeginning()));
	}

	return Result;
}

FString UBlueprintRegexBPLibrary::Replace(FString Input, FString Pattern, FString Replacement)
{
	FRegexMatcher Matcher(FRegexPattern(Pattern), Input);

	FString Result;

	int32 InputLen = Input.Len();
	int32 LastIndex = 0;

	while (Matcher.FindNext())
	{
		Result += Input.Mid(LastIndex, Matcher.GetMatchBeginning() - LastIndex);
		Result += Replacement;
		LastIndex = Matcher.GetMatchEnding();
	}

	if (LastIndex != InputLen)
	{
		Result += Input.Right(InputLen - LastIndex);
	}

	return Result;
}

FString UBlueprintRegexBPLibrary::Remove(FString Input, FString Pattern)
{
	return Replace(Input, Pattern, "");
}

TArray<FString> UBlueprintRegexBPLibrary::Split(FString Input, FString Pattern)
{
	TArray<FString> Result;

	FRegexMatcher Matcher(FRegexPattern(Pattern), Input);

	int32 InputLen = Input.Len();
	int32 LastIndex = 0;

	while (Matcher.FindNext())
	{
		Result.Add(Input.Mid(LastIndex, Matcher.GetMatchBeginning() - LastIndex));
		LastIndex = Matcher.GetMatchEnding();
	}

	if (LastIndex != InputLen)
	{
		Result.Add(Input.Right(InputLen - LastIndex));
	}
	return Result;
}

bool UBlueprintRegexBPLibrary::IsEmail(FString Input)
{
	return IsMatch(Input, "^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$");
}

bool UBlueprintRegexBPLibrary::IsIPv4Address(FString Input)
{
	return IsMatch(Input, "^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4}$");
}
