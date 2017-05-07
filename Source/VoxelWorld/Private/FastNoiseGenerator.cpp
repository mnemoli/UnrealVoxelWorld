#include "VoxelWorld.h"
#include "FastNoiseGenerator.h"
#include "UFNBlueprintFunctionLibrary.h"

UFastNoiseGenerator::UFastNoiseGenerator()
{
}

float UFastNoiseGenerator::GetNoise(float x, float y, float z)
{
	return Noise->GetNoise3D(x, y, z);
}

float UFastNoiseGenerator::GetNoise(float x, float y)
{
	return Noise->GetNoise2D(x, y);
}

void UFastNoiseGenerator::Setup(float Seed, float Octaves, float Frequency, float Scale)
{
	auto RootNoise = UUFNBlueprintFunctionLibrary::CreateFractalNoiseGenerator(this, EFractalNoiseType::FractalSimplex, Seed, Frequency, 0.5f, EInterp::InterpLinear, EFractalType::FBM, Octaves, 2.0f);
	Noise = UUFNBlueprintFunctionLibrary::CreateScaleBiasModule(this, RootNoise, Scale);
}