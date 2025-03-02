Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD;
};

float4 PSMain(VertexOut pin) : SV_Target
{
    return Texture.Sample(Sampler, pin.Texture);
}
