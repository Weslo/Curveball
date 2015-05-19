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

	bool Initialize(ID3D11Device* device, Material* _material);
	void Shutdown();
	bool Update(ID3D11DeviceContext* deviceContext, float dt);
	void Draw(ID3D11DeviceContext* deviceContext);

	Material* GetMaterial();
	int GetIndexCount();

	XMFLOAT4X4 GetWorldMatrix();
	void SetEmitterEnabled(bool _enabled);
	void SetEmitterPosition(XMFLOAT3 pos);

private:

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();

	void EmitParticles(float dt);
	void UpdateParticles(float dt);
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

	// This should only be calculated once and should just be a world matrix
	// that describes the origin transformation.
	XMFLOAT4X4 worldMatrix;
	bool emitterEnabled;
	XMFLOAT3 emitterPosition;
};

