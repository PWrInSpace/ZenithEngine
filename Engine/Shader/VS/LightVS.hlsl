#include "CBuffer/Transform.hlsli"

struct VSOut
{
	float3 texPos : TEX_POSITION;
	float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f), cb_transformViewProjection);
	vso.texPos = vso.pos.xyw;
	return vso;
}