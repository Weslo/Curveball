#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float2 lineBounds	: TEXCOORD1;
	float worldPos		: TEXCOORD2;
	float4 cameraPosition : TEXCOORD3;
};

struct Light
{
	float4 ambient : AMBIENT;
	float4 diffuse : DIFFUSE;
	float3 direction : DIRECTION;
	float range : TEXCOORD4;
	float3 position	: TEXCOORD5;
	float cone : TEXCOORD6;
	float3 attenuation : TEXCOORD7;
	int lightType : TEXCOORD8;
};

//L is vector from point to light
//N is normal of the point
float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	float4 diffuse = light.diffuse * NdotL;
	return diffuse + light.ambient;
}

//d is the distance from point to light source
float DoAttenuation(Light light, float d)
{
	return 1.0f / (light.attenuation[0] + light.attenuation[1] * d + light.attenuation[2] * d * d);
}

//V is the view vector, P is the point being shaded, N is the surface normal
float4 DoPointLight(Light light, float3 V, float4 P, float3 N)
{
	float4 result;

	float3 L = (float4(light.position, 1.0f) - P).xyz;
		float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	result = DoDiffuse(light, L, N) * attenuation;

	return result;
}

//V is the view vector, P is the point being shaded, N is the surface normal
float4 DoDirectionalLight(Light light, float3 V, float4 P, float3 N)
{
	float4 result;

	float3 L = -light.direction;

	result = DoDiffuse(light, L, N);

	return result;
}

//L is the light direction
float DoSpotCone(Light light, float3 L)
{
	float minCos = cos(light.cone);
	float maxCos = (minCos + 1.0f) / 2.0f;
	float cosAngle = dot(light.direction, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}

//V is the view vector, P is the point being shaded, N is the surface normal
float4 DoSpotLight(Light light, float3 V, float4 P, float3 N)
{
	float4 result;

	float3 L = (float4(light.position, 1.0f) - P).xyz;
		float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, L);

	result = DoDiffuse(light, L, N) * attenuation * spotIntensity;

	return result;
}

cbuffer perLight : register(b1)
{
	int numLights;
	float4 cameraPosition;
	Light lights[MAX_LIGHTS];
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	if (input.worldPos > input.lineBounds.x && input.worldPos < input.lineBounds.y)
	{
		return float4(0.4f, 0.6f, 0.75f, 0.0f);
	}

	input.normal = normalize(input.normal);

	float3 v = normalize(cameraPosition - input.position).xyz;

	float4 totalDiffuse = { 0, 0, 0, 0 };

	for (int i = 0; i < numLights; i++)
	{
		float4 diffuse = { 0, 0, 0, 0 };
				
		switch (lights[i].lightType)
		{
		case DIRECTIONAL_LIGHT:
		{
			diffuse = DoDirectionalLight(lights[i], v, input.position, input.normal);
		}
		break;

		case POINT_LIGHT:
		{
			diffuse = DoPointLight(lights[i], v, input.position, input.normal);
		}
		break;
		case SPOT_LIGHT:
		{
			diffuse = DoSpotLight(lights[i], v, input.position, input.normal);
		}
		break;
		}
				
		totalDiffuse += diffuse;
	}

	totalDiffuse = saturate(totalDiffuse);

	// Texture
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	return surfaceColor * totalDiffuse;
}