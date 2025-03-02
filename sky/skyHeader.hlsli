struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD;
    float3 normal : NORMAL;
};

struct Pxinput
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD;
    float3 normal : NORMAL;
    //float4x4 Matr : WORLDVIEWPROJ;

};
struct PS_OUTPUT
{
    float4 color : SV_TARGET;
    float depth : SV_DEPTH;
};
cbuffer cbPerObject : register(b0)
{
    float4x4 mProjectionMatrix;
    float4x4 View;
}
cbuffer Object : register(b1)
{
    float4x4 sky;
}
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);



float3 ReflectVector(float3 incident, float3 normal)
{
    // 计算反射向量


    return incident - 2.0f * abs(dot(incident, normal)) * normal;
}


