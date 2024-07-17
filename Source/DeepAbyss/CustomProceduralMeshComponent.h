// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CustomProceduralMeshComponent.generated.h"

UCLASS()
class DEEPABYSS_API UCustomProceduralMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	void GenerateLine(FVector StartPoint, FVector EndPoint, float Thickness);
};
