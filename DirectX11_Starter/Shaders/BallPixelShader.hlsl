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
	float4 worldPos		: TEXCOORD1;
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

struct LightResult
{
	float4 diffuse;
	float4 specular;
};

//L is vector from point to light
//N is normal of the point
float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	float4 diffuse = light.diffuse * NdotL;
	return diffuse + light.ambient;
}

float4 DoSpecular(Light light, float3 V, float3 L, float3 N)
{
	// Phong lighting.
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(R, V));

	// Blinn-Phong lighting
	float3 H = normalize(L + V);
	float NdotH = max(0, dot(N, H));

	return light.diffuse * pow(RdotV, 4);
}

//d is the distance from point to light source
float DoAttenuation(Light light, float d)
{
	return 1.0f / (light.attenuation[0] + light.attenuation[1] * d + light.attenuation[2] * d * d);
}

//V is the view vector, P is the point being shaded, N is the surface normal
LightResult DoPointLight(Light light, float3 V, float4 P, float3 N)
{
	LightResult res;
	float3 L = (float4(light.position, 1.0f) - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	res.diffuse = DoDiffuse(light, L, N) * attenuation;
	res.specular = DoSpecular(light, V, L, N) * attenuation;

	return res;
}

//V is the view vector, P is the point being shaded, N is the surface normal
LightResult DoDirectionalLight(Light light, float3 V, float4 P, float3 N)
{
	LightResult res;

	float3 L = -light.direction;

	res.specular = DoSpecular(light, V, L, N);
	res.diffuse = DoDiffuse(light, L, N);

	return res;
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
LightResult DoSpotLight(Light light, float3 V, float4 P, float3 N)
{
	LightResult res;

	float3 L = (float4(light.position, 1.0f) - P).xyz;
	float distance = length(L);
	//distance *= (distance / light.range);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, L);

	res.diffuse = DoDiffuse(light, L, N) * attenuation * spotIntensity;
	res.specular = DoSpecular(light, V, L, N) * attenuation * spotIntensity;

	return res;
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
	input.normal = normalize(input.normal);

	float3 v = normalize(cameraPosition - input.worldPos).xyz;

		LightResult totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			float4 diffuse = { 0, 0, 0, 0 };
			LightResult res = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
				if (lights[i].lightType != 99)
				{
					switch (lights[i].lightType)
					{
					case DIRECTIONAL_LIGHT:
					{
						res = DoDirectionalLight(lights[i], v, input.worldPos, input.normal);
					}
					break;

					case POINT_LIGHT:
					{
						res = DoPointLight(lights[i], v, input.worldPos, input.normal);
					}
					break;
					case SPOT_LIGHT:
					{
						res = DoSpotLight(lights[i], v, input.worldPos, input.normal);
					}
					break;
					}
				}
				totalResult.specular += res.specular;
				totalResult.diffuse += res.diffuse;
		}
	
	totalResult.specular = saturate(totalResult.specular);
	totalResult.diffuse = saturate(totalResult.diffuse);

	// Texture
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	return surfaceColor * (totalResult.diffuse + totalResult.specular);
}