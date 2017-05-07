#include "VoxelWorld.h"
#include "VoxelTerrain.h"
#include "RuntimeMeshComponent.h"


// Sets default values
AVoxelTerrain::AVoxelTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialise the dynamic mesh component
	TerrainMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Terrain Mesh"));

}

// Called when the game starts or when spawned
void AVoxelTerrain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoxelTerrain::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

