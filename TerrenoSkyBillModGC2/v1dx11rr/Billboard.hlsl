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


};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;

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


	vsOut.ApAmb = LuzAmbiental * FAA;
	vsOut.DireccionLuz = normalize(Direccion);


	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	//float3 ambient = float3(0.1f, 0.1f, 0.1f);
	float4 AportAmbiental = float4(pix.ApAmb, 0);

	float4 text = colorMap.Sample(colorSampler, pix.tex0);
	float4 textNorm = normalMap.Sample(colorSampler, pix.tex0);
	float3 bump = normalize(2.0 * textNorm - 1.0);
	float FALL = dot(normalize(bump), pix.DireccionLuz);
	float4 LuzDif = float4(LuzDifusa, 0);
	float4 AportLuzDif = saturate(LuzDif * FALL * FAD);



	if (text.a < 0.1)
	{
		clip(-1);
	}

	text = text * (AportAmbiental + AportLuzDif);
			
	return text;		
}