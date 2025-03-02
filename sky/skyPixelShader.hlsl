#include"skyHeader.hlsli"
PS_OUTPUT main(Pxinput vour)
{
    PS_OUTPUT PS_out;
    float exposure =1.5; 
    float gamma = 10.0f; 
    float4 v = Texture.Sample(Sampler, vour.Texture);

    //float Y = (0.299 * v.r )+ (0.587 * v.g) + (0.114 * v.b);
    //v *= pow(Y, gamma);
    //v *= exposure;
    //v = saturate(v);
    PS_out.color = v;
    PS_out.depth = 0.99999f;
    return PS_out;
}