// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkBase.h"

#include "ProceduralMeshComponent.h"
#include "RealtimeMeshComponent.h"
#include "RealtimeMeshSimple.h"
#include "Components/DynamicMeshComponent.h"
#include "DeepAbyss/FastNoiseLite.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "DynamicMesh/MeshAttributeUtil.h"

// Sets default values
AChunkBase::AChunkBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SetActorEnableCollision(true);
	
	//Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	DynamicMesh = CreateDefaultSubobject<UDynamicMeshComponent>("DynamicMesh");
	Noise = new FastNoiseLite();	

	//Mesh->SetCastShadow(false);

	SetRootComponent(DynamicMesh);
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

void AChunkBase::ApplyMesh()
{
	//Mesh->SetMaterial(0, Material);
	
	// enable proper UVs
	DynamicMeshDataHolder.EnableAttributes();
	DynamicMeshDataHolder.EnableVertexUVs(FVector2f::Zero());
	CopyVertexUVsToOverlay(DynamicMeshDataHolder, *DynamicMeshDataHolder.Attributes()->PrimaryUV()); // assuming we already generated vertices
	CopyVertexNormalsToOverlay(DynamicMeshDataHolder, *DynamicMeshDataHolder.Attributes()->PrimaryNormals());
	
	// enable Complex As Simple collision
	DynamicMesh->bEnableComplexCollision = true;
	DynamicMesh->CollisionType = CTF_UseComplexAsSimple;
	DynamicMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// set material
	DynamicMesh->SetMaterial(0, Material);
	
	// create our mesh
	DynamicMesh->SetMesh(MoveTemp(DynamicMeshDataHolder));
	
	//Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UVs,
	//	MeshData.Colors, TArray<FProcMeshTangent>() , true);
}

void AChunkBase::ClearMeshData()
{
	MeshData.Clear();
	VertexCount = 0;
}




