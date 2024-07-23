// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MineralBase.generated.h"

UCLASS()
class AMineralBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMineralBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Laser Pointer")
	FName MineralName;
	
	UPROPERTY(EditAnywhere, Category = "Appearance")
	FColor MineralColor;

	UPROPERTY(EditAnywhere, Category = "Appearance", meta=(ToolTip="Material used for spawning ores or spawning initial terrain"))
	UMaterialInstance* Material;

	UPROPERTY(EditAnywhere, Category = "Mineable")
	int Hardness;
};
