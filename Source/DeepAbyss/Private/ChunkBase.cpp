// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkBase.h"

#include "ProceduralMeshComponent.h"
#include "DeepAbyss/FastNoiseLite.h"

// Sets default values
AChunkBase::AChunkBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = new FastNoiseLite();	

	Mesh->SetCastShadow(false);

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AChunkBase::BeginPlay()
{
	Super::BeginPlay();

	Noise->SetFrequency(Frequency);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	
	GenerateHeightMap();

	GenerateMesh();

	ApplyMesh();
}

void AChunkBase::GenerateMesh()
{
	
}

void AChunkBase::GenerateHeightMap()
{
	
}

void AChunkBase::ApplyMesh() const
{
	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UVs,
		MeshData.Colors, TArray<FProcMeshTangent>() , true);
}



