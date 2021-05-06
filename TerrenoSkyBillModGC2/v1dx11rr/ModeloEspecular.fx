Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D especularMap : register(t2);

SamplerState colorSampler : register(s0);

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

cbuffer LuzAmb : register(b3)
{
	float3 LuzAmbiental;
	float FAA;
};
cbuffer LuzDif : register(b4)
{
	float3 LuzDifusa;
	float FAD;
};

cbuffer DirLuz : register(b5)
{
	float3 Direccion;
	float padding;
};


struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL0;
	uint3  aux  : COLOR0;
	float3 tangente : TANGENT0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 tangente : TEXCOORD2;
	float3 binormal : TEXCOORD3;

	float3 ApAmb : COLOR0;
	float3 DireccionLuz : COLOR1;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;
	vsOut.tangente = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangente = normalize(vsOut.tangente - vsOut.normal * dot(vsOut.normal, vsOut.tangente));
	vsOut.binormal = normalize(cross(vsOut.normal, vsOut.tangente));

	vsOut.ApAmb = LuzAmbiental * FAA;
	vsOut.DireccionLuz = normalize(Direccion);

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{


	///////////////////////////////////////////
	//APORTACION AMBIENTAL
	///////////////////////////////////////////
	float4 AportAmbiental = float4(pix.ApAmb, 0);
	float4 textColor = colorMap.Sample(colorSampler, pix.tex0);

	///////////////////////////////////////////
	//APORTACION DIFUSA
	///////////////////////////////////////////
	float4 textNorm = normalMap.Sample(colorSampler, pix.tex0);
	float3 bump = normalize(2.0 * textNorm - 1.0);
	float3x3 TBN = { pix.tangente, pix.binormal, pix.normal };
	float3 bumpTBN = mul(normalize(bump), TBN);
	float FALL = dot(normalize(bumpTBN), pix.DireccionLuz);
	float4 LuzDif = float4(LuzDifusa, 0);
	float4 AportLuzDif = saturate(LuzDif * FALL * FAD);

	///////////////////////////////////////////
	//APORTACION ESPECULAR
	///////////////////////////////////////////
	float4 textEspec = especularMap.Sample(colorSampler, pix.tex0);
	float4 LuzEspecular = { 1.0f, 0.9f, 1.0f, 1.0f };
	float3 camara = normalize(float3(-3, -5, 10));
	float FAS = 5;
	float3 Reflejo = normalize(2 * FALL * bumpTBN - pix.DireccionLuz);
	float4 AportEspecular = pow(saturate(dot(Reflejo, camara)), 30) * FAS * textEspec * LuzEspecular;

	textColor = textColor * (AportAmbiental + AportLuzDif + AportEspecular);


	return textColor;
}