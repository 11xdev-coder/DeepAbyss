// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkBase.h"
#include "FChunkMeshData.h"
#include "GameFramework/Actor.h"
#include "GreedyChunk.generated.h"

enum class EBlock;
class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class AGreedyChunk : public AChunkBase
{
	GENERATED_BODY()

	struct FMask
	{
		EBlock Block;
		int Normal;
	};
	
public:	
	// Sets default values for this actor's properties
	AGreedyChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<EBlock> Blocks;

	// methods
	void GenerateBlocks(); // populate blocks array

	void ApplyMesh() const;

	void GenerateMesh();

	void CreateQuad(FMask Mask, FIntVector AxisMask,
		int Width, int Height,
		FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);

	int GetBlockIndex(int X, int Y, int Z) const;

	EBlock GetBlock(FIntVector Index) const;

	bool CompareMask(FMask M1, FMask M2) const;
};
