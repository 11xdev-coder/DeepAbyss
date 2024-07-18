// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkBase.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

UCLASS()
class AChunkWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkWorld();

	UPROPERTY(EditInstanceOnly, Category = "Chunk World")
	TSubclassOf<AChunkBase> ChunkType;

	UPROPERTY(EditInstanceOnly, Category = "Chunk World")
	int DrawDistance = 5;

	UPROPERTY(EditInstanceOnly, Category = "Chunk World")
	int ChunkSize = 32;

	UPROPERTY(EditInstanceOnly, Category = "Chunk World")
	TObjectPtr<UMaterialInstance> ChunkMaterial;

	UPROPERTY(EditInstanceOnly, Category = "Noise Settings")
	float Frequency = 0.03f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
