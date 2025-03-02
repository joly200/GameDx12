
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

};
struct Light
{
    float4 position; // 光源位置
    float4 color; // 光源颜色
};
struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
    float padding[3];
};

cbuffer cbPerObject : register(b0)
{
    float4x4 mProjectionMatrix;
    float4x4 View;

}
cbuffer Object : register(b1)
{
    float4x4 fz;
}
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);


cbuffer LightingParams : register(b2)
{
    Light g_light;
    Material g_material;
    float3 g_viewPos; // 观察者位置

};
float3 ReflectVector(float3 incident, float3 normal)
{
    // 计算反射向量


    return incident - 2.0f * abs(dot(incident, normal)) * normal;
}
