#include "ParticleSystem.h"

// Default constructor initializes private members to 0.
ParticleSystem::ParticleSystem()
{
	material = 0;
	particles = 0;
	vertices = 0;
	vertexBuffer = 0;
	indexBuffer = 0;
}


ParticleSystem::~ParticleSystem()
{
}

bool ParticleSystem::Initialize(ID3D11Device* device, Material* _material)
{
	bool result;

	// Cache the material, make sure it exists.
	result = _material != 0;
	if (!result)
	{
		return false;
	}
	material = _material;

	// Initialize the particle system.
	result = InitializeParticleSystem();
	if (!result)
	{
		return false;
	}

	// Initialize the buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Successful initialization!
	return true;
}

// Releases buffers, particle system, and other data.
void ParticleSystem::Shutdown()
{
	// Release buffers.
	ShutdownBuffers();

	// Release particle system.
	ShutdownParticleSystem();
}

// Updates the particle system between frames.
bool ParticleSystem::Update(ID3D11DeviceContext* deviceContext, float dt)
{
	bool result;

	// Release old particles.
	KillParticles();

	// Emit new particles.
	if (emitterEnabled) { EmitParticles(dt); }

	// Update the position of live particles.
	UpdateParticles(dt);

	// Update the dynamic vertex buffer given the new position of each particle.
	result = UpdateBuffers(deviceContext);
	if (!result)
	{
		return false;
	}

	// Successful update!
	return true;
}

// Draws the particle system to the screen.
void ParticleSystem::Draw(ID3D11DeviceContext* deviceContext)
{
	// Calls the private method to render the buffers.
	RenderBuffers(deviceContext);
}

// Returns a reference to the material used for this particle system.
Material* ParticleSystem::GetMaterial()
{
	return material;
}

// Returns the number of indices used in this particle system's index buffer.
int ParticleSystem::GetIndexCount()
{
	return indexCount;
}

// Returns the world matrix used to draw this particle system.
XMFLOAT4X4 ParticleSystem::GetWorldMatrix()
{
	return worldMatrix;
}

// Sets whether or not the particle emitter is enabled.
void ParticleSystem::SetEmitterEnabled(bool _enabled)
{
	emitterEnabled = _enabled;
}

// Sets the position of the particle emitter.
void ParticleSystem::SetEmitterPosition(XMFLOAT3 pos)
{
	emitterPosition = pos;
}

// Initialize all the properties of the particle system to prepare it for updating.
bool ParticleSystem::InitializeParticleSystem()
{
	int i;

	// Initialize all the different elements used for particle processing.

	// Set position deviation.
	particleDeviationX = 0.1f;
	particleDeviationY = 0.1f;
	particleDeviationZ = 0.1f;
	
	// Set speed and speed deviation.
	particleVelocity = 1000.0f;
	particleVelocityVariation = 0.2f;

	// Set the size of the particles.
	particleSize = 0.015f;

	// Set system emission rate.
	particlesPerSecond = 100.0f;

	// Set max living particles.
	maxParticles = 5000;

	// Initialize the array of particles based on the maximum number of living particles.
	particles = new ParticleType[maxParticles]; // TODO: Delete on cleanup.
	if (!particles)
	{
		return false;
	}

	// Deactivate each particle in the list for startup.
	for (i = 0; i < maxParticles; i++)
	{
		particles[i].active = false;
	}

	// Reset system counters.
	currentParticleCount = 0;
	accumulatedTime = 0;


	// Initialize the world matrix.
	XMMATRIX translationMatrix = XMMatrixTranslation(0, 0, 0);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(0, 0, 0);
	XMMATRIX scaleMatrix = XMMatrixScaling(1, 1, 1);
	XMMATRIX transformationMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(transformationMatrix));

	// Initialize the particle emitter.
	emitterEnabled = false;
	emitterPosition = XMFLOAT3(0, 0, 0);

	// Successfully initialized particle system!
	return true;
}

// Cleans up data related to the particle system.
void ParticleSystem::ShutdownParticleSystem()
{
	// Release the particle list
	if (particles)
	{
		delete[] particles;
		particles = 0;
	}
}

// Prepares the vertex and index buffers to be used for rendering.
bool ParticleSystem::InitializeBuffers(ID3D11Device* device)
{
	unsigned long* indices;
	int i;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set max vertices and indices.
	vertexCount = maxParticles * 6;
	indexCount = vertexCount;

	// Create vertex array for particles being rendered.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeroes by default.
	memset(vertices, 0, (sizeof(VertexType)* vertexCount));

	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize index array.
	for (i = 0; i < indexCount; i++)
	{
		indices[i] = i;
	}

	// Setup the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release index array since we don't need it anymore.
	delete[] indices;
	indices = 0;

	// Successfully initialized buffers!
	return true;
}

// Cleans up data related to the vertex and index buffers.
void ParticleSystem::ShutdownBuffers()
{
	// Release the index buffer.
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}
	
	// Release the vertex buffer.
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}

	// Release vertex array since we don't need it anymore.
	delete[] vertices;
	vertices = 0;
}

// Emits a burst of numParticles particles at position pos.
void ParticleSystem::BurstEmitParticles(int numParticles, XMFLOAT3 pos)
{
	int i;

	for (i = 0; i < numParticles; i++)
	{
		if (currentParticleCount < (maxParticles - 1))
		{
			BurstEmitSingleParticle(pos);
		}
	}
}

// Emits a single particle at position pos.
void ParticleSystem::BurstEmitSingleParticle(XMFLOAT3 pos)
{
	float _x, _y, _z, _velocity, _r, _g, _b;
	int i, j, index;
	bool found;

	// Increment particle count.
	currentParticleCount++;

	// Generate randomized particle properties.
	_x = (((float)rand() - (float)rand()) / RAND_MAX) * particleDeviationX;
	_y = (((float)rand() - (float)rand()) / RAND_MAX) * particleDeviationY;
	_z = (((float)rand() - (float)rand()) / RAND_MAX) * particleDeviationZ;

	_velocity = particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * particleVelocityVariation;

	_r = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
	_g = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
	_b = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

	// Position the new particle relative to the emitter.
	_x += pos.x;
	_y += pos.y;
	_z += pos.z;

	// Sort the new particle by depth.
	index = 0;
	found = false;
	while (!found)
	{
		// If the particle we are inspecting is inactive or it's z coord is less
		// than the incoming one, we've found our location.
		if ((particles[index].active == false) || (particles[index].z < _z))
		{
			found = true;
		}
		else
		{
			// Otherwise check the next index.
			index++;
		}
	}

	// Now that we've found the correct index for this new particle, we
	// need to make room by copying the array over one position and inserting
	// data for the new particle.
	i = currentParticleCount;
	j = i - 1;

	// Copy data over one index.
	while (i != index)
	{
		particles[i].x = particles[j].x;
		particles[i].y = particles[j].y;
		particles[i].z = particles[j].z;
		particles[i].r = particles[j].r;
		particles[i].g = particles[j].g;
		particles[i].b = particles[j].b;
		particles[i].velocity = particles[j].velocity;
		particles[i].active = particles[j].active;

		i--;
		j--;
	}

	// Insert new particle data at the correct index.
	particles[index].x = _x;
	particles[index].y = _y;
	particles[index].z = _z;
	particles[index].r = _r;
	particles[index].g = _g;
	particles[index].b = _b;
	particles[index].velocity = _velocity;
	particles[index].active = true;
}

// Called each frame to emit new particles based on the change in time between the previous and
// new frames. Ensures the proper number of particles are emitted and properly sorted by depth.
void ParticleSystem::EmitParticles(float dt)
{
	bool emitParticle;

	// Increment accumulated time.
	accumulatedTime += dt;

	// Initialize emitParticle to false.
	emitParticle = false;

	// Check if there is time to emit a new particle.
	if (accumulatedTime > 1.0f / particlesPerSecond)
	{
		accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// If there are particles to emit, then emit one per frame.
	if ((emitParticle == true) && (currentParticleCount < (maxParticles - 1)))
	{
		BurstEmitSingleParticle(emitterPosition);
	}
}

// Update the properties of the particles each frame.
void ParticleSystem::UpdateParticles(float dt)
{
	int i;

	// Update each particle.
	for (i = 0; i < currentParticleCount; i++)
	{
		particles[i].y = particles[i].y - (particles[i].velocity * dt * 0.001f);
	}
}

// Removes particles from the system that have exceeded their lifetime.
void ParticleSystem::KillParticles()
{
	int i, j;

	// Kill particles that have moved past the height of -3.0f.
	for (i = 0; i < maxParticles; i++)
	{
		if ((particles[i].active == true) && (particles[i].y < -1.5f))
		{
			particles[i].active = false;
			currentParticleCount--;

			// Shift all the live particles in the array.
			for (j = i; j < maxParticles - 1; j++)
			{
				particles[j].x = particles[j + 1].x;
				particles[j].y = particles[j + 1].y;
				particles[j].z = particles[j + 1].z;
				particles[j].r = particles[j + 1].r;
				particles[j].g = particles[j + 1].g;
				particles[j].b = particles[j + 1].b;
				particles[j].velocity = particles[j + 1].velocity;
				particles[j].active = particles[j + 1].active;
			}
		}
	}
}

// Immediately kills all living particles.
void ParticleSystem::KillAllParticles()
{
	int i;

	// Kill all the particles ever.
	for (i = 0; i < maxParticles; i++)
	{
		particles[i].active = false;
	}
	currentParticleCount = 0;
}

// Updates the dynamic vertex buffer each frame.
bool ParticleSystem::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	int index, i;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Initialize vertex array to zero.
	memset(vertices, 0, sizeof(VertexType)* vertexCount);

	// Build the vertex array from the particle list.
	index = 0;

	for (i = 0; i < currentParticleCount; i++)
	{
		// Bottom left.
		vertices[index].position = XMFLOAT3(particles[i].x - particleSize, particles[i].y - particleSize, particles[i].z);
		vertices[index].texture = XMFLOAT2(0, 1);
		vertices[index].color = XMFLOAT4(particles[i].r, particles[i].g, particles[i].b, 1);
		index++;

		// Top Left.
		vertices[index].position = XMFLOAT3(particles[i].x - particleSize, particles[i].y + particleSize, particles[i].z);
		vertices[index].texture = XMFLOAT2(0, 0);
		vertices[index].color = XMFLOAT4(particles[i].r, particles[i].g, particles[i].b, 1);
		index++;

		// Bottom Right.
		vertices[index].position = XMFLOAT3(particles[i].x + particleSize, particles[i].y - particleSize, particles[i].z);
		vertices[index].texture = XMFLOAT2(1, 1);
		vertices[index].color = XMFLOAT4(particles[i].r, particles[i].g, particles[i].b, 1);
		index++;
		
		// Bottom Right.
		vertices[index].position = XMFLOAT3(particles[i].x + particleSize, particles[i].y - particleSize, particles[i].z);
		vertices[index].texture = XMFLOAT2(1, 1);
		vertices[index].color = XMFLOAT4(particles[i].r, particles[i].g, particles[i].b, 1);
		index++;

		// Top Left.
		vertices[index].position = XMFLOAT3(particles[i].x - particleSize, particles[i].y + particleSize, particles[i].z);
		vertices[index].texture = XMFLOAT2(0, 0);
		vertices[index].color = XMFLOAT4(particles[i].r, particles[i].g, particles[i].b, 1);
		index++;

		// Top Right.
		vertices[index].position = XMFLOAT3(particles[i].x + particleSize, particles[i].y + particleSize, particles[i].z);
		vertices[index].texture = XMFLOAT2(1, 0);
		vertices[index].color = XMFLOAT4(particles[i].r, particles[i].g, particles[i].b, 1);
		index++;
	}

	// Lock the vertex buffer.
	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)* vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(vertexBuffer, 0);

	// Successful buffer update!
	return true;
}

// Draws the particle buffers.
void ParticleSystem::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex data into the input assembler so shaders can render it.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index data into the input assembler so shaders can render it.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}