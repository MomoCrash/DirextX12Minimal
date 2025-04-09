cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float4 color;
};

cbuffer cbPass : register(b1)
{
    float4x4 gViewProj;
};

struct VS_VertexIn
{
    float3 PosL : POSITION;
    float3 Norm : NORMAL;
    float3 Tangent : TANGENT;
    float2 UV : TEXCOORD;
};

struct VS_VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
};

VS_VertexOut VS(VS_VertexIn vin)
{
    VS_VertexOut vout;
    
    // Transform to homogeneous clip space.
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosH = mul(posW, gViewProj);
    
    vout.PosW = posW.xyz;
	
    return vout;
}