#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState basicSampler : register(s0);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float4 world		: TEXCOORD1;
	float2 screenUV		: TEXCOORD2;
};

struct Light
{
	float4 ambient;
	float4 diffuse;
	float3 direction;
	float range;
	float3 position;
	float cone;
	float3 attenuation;
	int lightType;
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

cbuffer perLight : register(b0)
{
	float4 cameraPosition;
	Light lights[MAX_LIGHTS];
};

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	float3 v = normalize(cameraPosition - input.world).xyz;

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
					diffuse = DoDirectionalLight(lights[i], v, input.world, input.normal);
				}
				break;

				case POINT_LIGHT:
				{
					diffuse = DoPointLight(lights[i], v, input.world, input.normal);
				}
				break;
				case SPOT_LIGHT:
				{
					diffuse = DoSpotLight(lights[i], v, input.world, input.normal);
				}
				break;
			}
		}
		totalDiffuse += diffuse;
	}

	totalDiffuse = saturate(totalDiffuse);

	//
	// REFRACTION
	//

	float3 normalMap = normalTexture.Sample(basicSampler, input.uv).xyz * 2 - 1;
	float3 adjustedNormal = normalize(input.normal + normalMap * 0.1f);


	// Dir light spec
	float3 toCamera = normalize(cameraPosition - input.world);
	float3 refl = reflect(normalize(direction), adjustedNormal);
	specular = pow(max(dot(refl, toCamera), 0), 8);


	// Refract and alter the final uv for refraction
	float3 refraction = refract(-toCamera, adjustedNormal, 0.9f);
	input.screenUV.x += refraction.x * 0.5f;
	input.screenUV.y += refraction.y * 0.5f;

	// Texture
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.screenUV);

	return surfaceColor * totalDiffuse;
}