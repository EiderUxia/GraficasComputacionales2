Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);

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
	uint3  auxes : COLOR0;
	float3 tangente : TEXCOORD2;
	float3 binormal : TEXCOORD3;
	float3 LAmb : TEXCOORD4;
	float Faa : TEXCOORD5;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;	
	/*
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));	
	vsOut.auxes = vertex.aux;*/
	vsOut.tangente = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangente = normalize(vsOut.tangente - vsOut.normal * dot(vsOut.normal, vsOut.tangente));
	vsOut.binormal = normalize(cross(vsOut.normal, vsOut.tangente));

	vsOut.LAmb = LuzAmbiental;
	vsOut.Faa = FAA;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	///////////////////////////////////////////
	//APORTACION AMBIENTAL
	///////////////////////////////////////////

	float4 LuzAmbiental = float4 (pix.LAmb, 1.0);
	float FAA = pix.Faa;  //se controla del cpu segun la hora del dia
	float4 AportAmbiental;
	AportAmbiental = LuzAmbiental * FAA;
	float4 textColor = colorMap.Sample(colorSampler, pix.tex0);
	
	///////////////////////////////////////////
	//APORTACION DIFUSA
	///////////////////////////////////////////
	float3 DirLuz = float3(5, 10, 2);
	float4 LuzDifusa = float4(1, 1, 1, 1);
	DirLuz = normalize(DirLuz);
	float FAD = 0.5;
	float4 textNorm = normalMap.Sample(colorSampler, pix.tex0);
	float3 bump = normalize(2.0 * textNorm - 1.0);
	float3x3 TBN = { pix.tangente, pix.binormal, pix.normal };
	float3 bumpTBN = mul(normalize(bump), TBN);
	float FALL = dot(normalize(bumpTBN), DirLuz);
	float4 AportLuzDif = saturate(LuzDifusa * FALL * FAD);

	textColor = textColor * (AportAmbiental + AportLuzDif);
	//textColor = textColor * (AportAmbiental);

	return textColor;
}