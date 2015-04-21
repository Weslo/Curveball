#pragma once
#include"DirectXGame.h"
#include<DirectXMath.h>
#include"Material.h"

using namespace DirectX;

class ParticleSystem
{
private:

	// Defines properties for a single particle.
	struct ParticleType
	{
		float x, y, z;
		float r, g, b;
		float velocity;
		bool active;
	};

	// Vertex struct for particles.
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};

public:
	ParticleSystem();
	~ParticleSystem();

	bool InitializeParticleSystem();
	void ShutDownParticleSystem();

	bool InitializeBuffers(ID3D11Device* device);
	void ShutDownBuffers();

	void EmitParticles(float dt);
	void UpdatParticles(float dt);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext* deviceContext);

	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:

	// Particle system properties.
	float particleDeviationX, particleDeviationY, particleDeviationZ;
	float particleVelocity, particleVelocityVariation;
	float particleSize, particlesPerSecond;
	int maxParticles;

	// State tracking.
	int currentParticleCount;
	float accumulatedTime;

	// Single material for all particles.
	Material* material;

	// List of all the particles.
	ParticleType* particles;

	// Particles are rendered using a single vertex/index buffer.
	// TODO: Maybe abstract into Mesh* object.
	int vertexCount, indexCount;
	VertexType* vertices;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};

