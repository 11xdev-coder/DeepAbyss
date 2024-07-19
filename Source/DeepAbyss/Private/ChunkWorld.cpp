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
				SpawnedChunk->Material = ChunkMaterial;
			
				UGameplayStatics::FinishSpawningActor(SpawnedChunk, Transform);
			}	
		}
	}	
}

