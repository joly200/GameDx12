
#include"fzHeader.hlsli"
float4 main(Pxinput pos) : SV_Target
{

    float4 ret = { 0.0, 0.0, 0.0, 0.0 };

    float3 dI = pos.PosL - g_light.position.xyz;
    float3 nI = normalize(dI);

    float zet = dot(-nI, normalize(pos.normal));

    float qd = zet / pow(length(dI), 2);
    //ret = (g_material.shininess * qd + g_material.ambient) * Texture.Sample(Sampler, pos.Texture);
    float4 Texture1 = Texture.Sample(Sampler, pos.Texture);
    return (zet > 0.0f) ? g_material.shininess * Texture1 + g_material.ambient * qd * Texture1 : g_material.ambient * Texture1;
    
}