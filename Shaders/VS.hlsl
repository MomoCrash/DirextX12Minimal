// INFORMATION d'un OBJECT
cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
};

// INFORMATION du MONDE
cbuffer cbPass : register(b1)
{
    float4x4 gViewProj;
};

// INFORMATION d'une FACE
// Utilise l'Input layout :
struct VS_VertexIn
{
    float3 PosL : POSITION;
    float4 color : COLOR;
};

struct VS_VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float4 colorOut : COLOR;
};

VS_VertexOut VS(VS_VertexIn vin)
{
    VS_VertexOut vout;
    
    // Transform to homogeneous clip space.
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosH = mul(posW, gViewProj);
    vout.PosW = posW.xyz;
    
    vout.colorOut = vin.color;
    
    return vout;
}