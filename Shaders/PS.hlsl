cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
};

cbuffer cbPass : register(b1)
{
    float4x4 gViewProj;
};

struct VS_VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float4 colorOut : COLOR;
};

float4 PS(VS_VertexOut pin) : SV_Target
{
    return pin.colorOut;
}