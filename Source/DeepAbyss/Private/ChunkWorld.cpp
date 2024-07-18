// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"

#include "ShaderCompiler.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	for (int x = -DrawDistance; x <= DrawDistance; ++x)
	{
		for (int y = -DrawDistance; y <= DrawDistance; ++y)
		{
			for (int z = -DrawDistance; z <= DrawDistance; ++z)
			{
				auto Transform = FTransform(
					FRotator::ZeroRotator,
					FVector(x * ChunkSize * 100, y * ChunkSize * 100, z * ChunkSize * 100),
					FVector::OneVector
				);

				auto SpawnedChunk = GetWorld()->SpawnActorDeferred<AChunkBase>(
					ChunkType,
					Transform,
					this
				);

				SpawnedChunk->ChunkSize = ChunkSize;
				SpawnedChunk->Frequency = Frequency;
				SpawnedChunk->Material = ChunkMaterial;
				
				UGameplayStatics::FinishSpawningActor(SpawnedChunk, Transform);
			}			
		}
	}
}

