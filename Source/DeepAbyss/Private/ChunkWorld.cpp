// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"

#include "EngineUtils.h"
#include "ShaderCompiler.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
	PrimaryActorTick.bCanEverTick = false;

	int NumChunks = (2 * DrawDistance + 1) * (2 * DrawDistance + 1);
	Chunks.SetNum(NumChunks);
}


// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	if(Generate)
	{
		for (int x = -DrawDistance; x <= DrawDistance; ++x)
		{
			for (int y = -DrawDistance; y <= DrawDistance; ++y) 
			{
				auto Transform = FTransform(
					FRotator::ZeroRotator,
					FVector(x * ChunkSize.X * 100, y * ChunkSize.Y * 100, ChunkSize.Z * 100), // z is vertical
					FVector::OneVector
				);

				auto SpawnedChunk = GetWorld()->SpawnActorDeferred<AChunkBase>(
					ChunkType,
					Transform,
					this
				);

				SpawnedChunk->ChunkSize = ChunkSize;
				SpawnedChunk->Frequency = Frequency;
				SpawnedChunk->Material = DefaultMineral.GetDefaultObject()->Material;
				SpawnedChunk->DefaultMineral = DefaultMineral.GetDefaultObject();
			
				UGameplayStatics::FinishSpawningActor(SpawnedChunk, Transform);

				Chunks[GetChunkIndex(x, y)] = SpawnedChunk;
			}	
		}
	}	
}

int AChunkWorld::GetChunkIndex(int X, int Y)
{
	int OffsetX = X + DrawDistance;
	int OffsetY = Y + DrawDistance;
	return OffsetY * (2 * DrawDistance + 1) + OffsetX;
}

TArray<AChunkBase*> AChunkWorld::GetNeighboringChunks(int X, int Y)
{
	TArray<AChunkBase*> NeighboringChunks;
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (dx == 0 && dy == 0) continue; // Skip the center chunk

			int NeighborX = X + dx;
			int NeighborY = Y + dy;

			// Ensure the neighbor indices are within the valid range
			if (NeighborX >= -DrawDistance && NeighborX <= DrawDistance &&
				NeighborY >= -DrawDistance && NeighborY <= DrawDistance)
			{
				int Index = GetChunkIndex(NeighborX, NeighborY);
				if (Chunks.IsValidIndex(Index))
				{
					AChunkBase* NeighborChunk = Chunks[Index];
					if (NeighborChunk)
					{
						NeighboringChunks.Add(NeighborChunk);
					}
				}
			}
		}
	}
	return NeighboringChunks;
}
