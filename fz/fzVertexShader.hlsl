#include"fzHeader.hlsli"


Pxinput main(VertexIn pos) //: SV_POSITION
{
    Pxinput vout;
    vout.PosH = mul(fz, float4(pos.PosL, 1.0f));
    vout.PosL = mul((float3x3) fz, pos.PosL);
    vout.PosH.xyzw /= vout.PosH.w;
    vout.PosH = mul(View, vout.PosH);

    vout.PosH = mul(mProjectionMatrix, vout.PosH);
    vout.Color = pos.Color;
    vout.Texture = pos.Texture;
    vout.normal = normalize(mul(pos.normal, (float3x3) fz));
    return vout;
}