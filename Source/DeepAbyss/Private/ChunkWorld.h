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

	TArray<AChunkBase*> Chunks;
	
	UPROPERTY(EditInstanceOnly, Category = "Chunk World")
	bool Generate = true;

	UPROPERTY(EditInstanceOnly, Category = "Chunk World")
	TSubclassOf<AChunkBase> ChunkType;

	UPROPERTY(EditInstanceOnly, Category = "Chunk World")
	int DrawDistance = 5;

	UPROPERTY(EditInstanceOnly, Category = "Chunk World")
	FIntVector ChunkSize = FIntVector(256, 256, 256);

	UPROPERTY(EditAnywhere, Category = "Appearance")
	TSubclassOf<AMineralBase> DefaultMineral;
	
	UPROPERTY(EditInstanceOnly, Category = "Noise Settings")
	float Frequency = 0.03f;

	int GetChunkIndex(int X, int Y);

	TArray<AChunkBase*> GetNeighboringChunks(int X, int Y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
