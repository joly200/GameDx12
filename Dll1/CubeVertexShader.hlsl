
cbuffer cbPerObject : register(b0)
{
    float4x4 mProjectionMatrix;
    float4x4 View;

}
cbuffer Object : register(b1)
{
    float4x4 Cuba;
}
struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD;
};

VertexOut VSMain(VertexIn vin)
{
    VertexOut vout;
    vout.PosH = mul(Cuba, float4(vin.PosL, 1.0f));
    vout.PosH.xyzw /= vout.PosH.w;
    vout.PosH = mul(View, vout.PosH);

    vout.PosH = mul(mProjectionMatrix, vout.PosH);

    vout.Color = vin.Color;
    vout.Texture = vin.Texture;
    return vout;
}