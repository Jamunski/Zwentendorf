// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FloatUtilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ZWENTENDORF_API UFloatUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	UFUNCTION(BlueprintCallable, Category = "FloatUtility")
	static FORCEINLINE float GetFloatWithPrecision(float value, int32 precision)
	{
		return (floor((value * pow(10, precision) + 0.5)) / pow(10, precision));
	}

	UFUNCTION(BlueprintCallable, Category = "FloatUtility")
	static FORCEINLINE FString GetFloatAsStringWithPrecision(float value, int32 precision, bool bIncludeLeadingZero = true)
	{
		float precisionFloat = GetFloatWithPrecision(value, precision);

		FNumberFormattingOptions NumberFormat; //Text.h
		NumberFormat.MinimumIntegralDigits = (bIncludeLeadingZero) ? 1 : 0;
		NumberFormat.MaximumIntegralDigits = 10000;
		NumberFormat.MinimumFractionalDigits = precision;
		NumberFormat.MaximumFractionalDigits = precision;
		return FText::AsNumber(precisionFloat, &NumberFormat).ToString();
	}

	UFUNCTION(BlueprintCallable, Category = "FloatUtility")
	static FORCEINLINE FText GetFloatAsTextWithPrecision(float value, int32 precision, bool bIncludeLeadingZero = true)
	{
		float precisionFloat = GetFloatWithPrecision(value, precision);

		FNumberFormattingOptions NumberFormat; //Text.h
		NumberFormat.MinimumIntegralDigits = (bIncludeLeadingZero) ? 1 : 0;
		NumberFormat.MaximumIntegralDigits = 10000;
		NumberFormat.MinimumFractionalDigits = precision;
		NumberFormat.MaximumFractionalDigits = precision;
		return FText::AsNumber(precisionFloat, &NumberFormat);
	}
};
