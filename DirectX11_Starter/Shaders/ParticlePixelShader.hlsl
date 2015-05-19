#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D particleTexture;
SamplerState samplerState;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
	float4 worldPos : TEXCOORD1;
	float3 normal : NORMAL;
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

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 finalColor;

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

	textureColor = particleTexture.Sample(samplerState, input.tex);

	finalColor = textureColor * input.color * (totalResult.diffuse + totalResult.specular) + float4(.2f, .2f, .2f, 1.0f);

	return finalColor;
}