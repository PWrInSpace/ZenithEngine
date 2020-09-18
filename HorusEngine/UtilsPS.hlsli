struct LightVectorData
{
	float3 vertexToLight;
	float distanceToLight;
	float3 directionToLight;
};

LightVectorData GetLightVectorData(uniform float3 lightPos, const in float3 vertexPos)
{
	LightVectorData lvd;
	lvd.vertexToLight = lightPos - vertexPos;
	lvd.distanceToLight = length(lvd.vertexToLight);
	lvd.directionToLight = lvd.vertexToLight / lvd.distanceToLight;
	return lvd;
}

float3 GetMappedNormal(const in float3 tan, const in float3 bitan, const in float3 normal,
	const in float2 texcoord, uniform Texture2D normalMap, uniform SamplerState splr)
{
	// Get rotation from tangent space
	const float3x3 tangentToView = float3x3
		(
			normalize(tan),
			normalize(bitan),
			normalize(normal)
		);
	// Sample normal to tangent space
	const float3 tangentNormal = normalMap.Sample(splr, texcoord).rgb * 2.0f - 1.0f;
	// Transform from tangent into view space
	return normalize(mul(tangentNormal, tangentToView));
}

float GetAttenuation(uniform float attConst, uniform float attLinear, uniform float attQuad, const in float distanceToLight)
{
	// Diffuse attenuation
	// http://wiki.ogre3d.org/-Point+Light+Attenuation
	return 1.0f / (attConst + (attLinear + attQuad * distanceToLight) * distanceToLight);
}

float3 GetDiffuse(const in float3 diffuseColor, const in float3 directionToLight, const in float3 normal, const in float attenutaion)
{
	return diffuseColor * (attenutaion * max(0.0f, dot(directionToLight, normal)));
}

float3 GetSpecular(uniform float3 cameraPos, const in float3 directionToLight, const in float3 pos, const in float3 normal, const in float attenuation,
	const in float3 specularColor, const in float specularPower)
{
	// Halfway vector between directionToLight and directionToCamera
	const float3 H = normalize(normalize(cameraPos - pos) + directionToLight);
	return specularColor * (attenuation * pow(saturate(dot(normal, H)), specularPower));
}

float GetSampledSpecularPower(const in float4 specularData)
{
	// https://gamedev.stackexchange.com/questions/74879/specular-map-what-about-the-specular-reflections-highlight-size
	return pow(2.0f, specularData.a * 13.0f);
}

float3 GetWorldPosition(const in float2 texCoord, const in float depth, uniform matrix inverseViewProjection)
{
	const float x = texCoord.x * 2.0f - 1.0f;
	const float y = (1.0f - texCoord.y) * 2.0f - 1.0f;
	const float4 pos = mul(float4(x, y, depth, 1.0f), inverseViewProjection);
	return pos.xyz / pos.w;
}

float GetShadowLevel(const in float3 pos, uniform float3 lightPos, uniform SamplerComparisonState shadowSplr, uniform TextureCube shadowMap)
{
	const float3 shadowPos = pos - lightPos;
	const float len = sqrt(length(shadowPos));
	return shadowMap.SampleCmpLevelZero(shadowSplr, shadowPos, len / (len + 1.0f));

	//return saturate(1.0f - exp(e_x)); //exp(30.0f * (saturate(length(shadowPos) / 1000.0f) - shadowMap.Sample(shadowSplr, shadowPos).x));
	//const float3 shadowUVZ = shadowPos.xyz / shadowPos.w; // Perspecitve divide
	//float level = 0.0f;
	//if (shadowUVZ.z > 1.0f || shadowUVZ.z < 0.0f)
	//	level = 1.0f;
	//else
	//{
	//	// PCF anti-aliasing https://developer.nvidia.com/gpugems/gpugems/part-ii-lighting-and-shadows/chapter-11-shadow-map-antialiasing
	//	static const int PCF_RANGE = 2;
	//	[unroll]
	//	for (int x = -PCF_RANGE; x <= PCF_RANGE; ++x)
	//	{
	//		[unroll]
	//		for (int y = -PCF_RANGE; y <= PCF_RANGE; ++y)
	//			level += shadowMap.SampleCmpLevelZero(shadowSplr, shadowUVZ.xy, shadowUVZ.z, int2(x, y)); // Hardware comparison (hardware PCF 2x2 grid)
	//	}
	//	level /= (PCF_RANGE * 2 + 1) * (PCF_RANGE * 2 + 1);
	//}
	//return level;
}