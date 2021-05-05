Texture2D colorMap : register(t0);
Texture2D colorMap2 : register(t1);
Texture2D blendMap : register(t2);
SamplerState colorSampler : register(s0);

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
Texture2D TexTerr1 : register(t3);
Texture2D TexTerr2 : register(t4);
Texture2D TexTerr3 : register(t5);

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


cbuffer cbChangerEveryFrame : register(b0)
{
	matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
	matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
	matrix projMatrix;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangente : NORMAL1;
	float3 binormal : NORMAL2;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangent : NORMAL1;
	float3 binorm : NORMAL2;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;
	vsOut.blendTex = vertex.blendTex;
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangent = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.binorm = normalize(mul(vertex.binormal, worldMatrix));

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	
	float4 fColor;

	float4 Grass = TexTerr1.Sample(colorSampler, pix.tex0);
	float4 Ground = TexTerr2.Sample(colorSampler, pix.tex0);
	float4 Pebbles = TexTerr3.Sample(colorSampler, pix.tex0);
	float4 blend = blendMap.Sample(colorSampler, pix.blendTex);

	fColor = lerp(Grass, Pebbles, blend.r);
	fColor = lerp(fColor, Ground, blend.b);

	return fColor;
}