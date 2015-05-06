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
	float4 worldPos		: TEXCOORD2;
	float4 tint			: COLOR;
};

struct Light
{
	float4 ambient;
	//------
	float4 diffuse;
	//------
	float3 direction;
	float range;
	//------
	float3 position;
	float cone;
	//------
	float3 attenuation;
	int lightType;
	//------
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
	float3 L = (float4(light.position, 1.0f) - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	return (DoDiffuse(light, L, N) * attenuation);
}

//V is the view vector, P is the point being shaded, N is the surface normal
float4 DoDirectionalLight(Light light, float3 V, float4 P, float3 N)
{
	float3 L = -light.direction;

	return  DoDiffuse(light, L, N);
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
	float3 L = (float4(light.position, 1.0f) - P).xyz;
		float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, L);

	return (DoDiffuse(light, L, N) * attenuation * spotIntensity);
}

cbuffer perLight : register(b0)
{
	float4 cameraPosition;
	Light lights[MAX_LIGHTS];
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	if (input.worldPos.z > input.lineBounds.x && input.worldPos.z < input.lineBounds.y)
	{
		return float4(0.4f, 0.6f, 0.75f, 0.0f);
	}

	input.normal = normalize(input.normal);

	float3 v = normalize(cameraPosition - input.worldPos).xyz;

	float4 totalDiffuse = { 0, 0, 0, 0 };

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		float4 diffuse = { 0, 0, 0, 0 };
				
		if (lights[i].lightType != 99)
		{
			switch (lights[i].lightType)
			{
			case DIRECTIONAL_LIGHT:
			{
									  diffuse = DoDirectionalLight(lights[i], v, input.worldPos, input.normal);
			}
				break;

			case POINT_LIGHT:
			{
								diffuse = DoPointLight(lights[i], v, input.worldPos, input.normal);
			}
				break;
			case SPOT_LIGHT:
			{
							   diffuse = DoSpotLight(lights[i], v, input.worldPos, input.normal);
			}
				break;
			}
		}
		totalDiffuse += diffuse;
	}

	totalDiffuse = saturate(totalDiffuse);

	// Texture
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	if (surfaceColor.x == 0 && surfaceColor.y == 0 && surfaceColor.z == 0)
	{
		return totalDiffuse;
	}

	surfaceColor += input.tint * 0.5f;

	return surfaceColor * totalDiffuse;
}