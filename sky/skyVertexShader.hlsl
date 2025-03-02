#include"skyHeader.hlsli"
Pxinput main(VertexIn pos)
{
    Pxinput vout;
    //float4x4 cameraMatrix = float4x4(
    //    1.0f, 0.0f, 0.0f, View[3].x,
    //    0.0f, 1.0f, 0.0f, View[3].y,
    //    0.0f, 0.0f, 1.0f, View[3].z,
    //    0.0f, 0.0f, 0.0f, 1.0f
    //);
    //float4x4 cameraMatrix = float4x4(
    //    1.0f, 0.0f, 0.0f, 0.0f,
    //    0.0f, 1.0f, 0.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f, 0.0f,
    //    0.1, 1230.0,1233.0, 1.0f
    //);
    float4x4 newView = View;
    newView[0][3] = 0.0f;
    newView[1][3] = 0.0f;
    newView[2][3] = 0.0f;
    newView[3][3] = 1.0f;
    vout.PosH = mul(newView, float4(pos.PosL, 1.0f));
    //vout.Matr = newView;
    //
    //newView[3].x = 0.0f;
    //newView[3].y = 0.0f;
    //newView[3].z = 0.0f;
    vout.PosH.xyzw /= vout.PosH.w;
    //vout.PosH = mul(newView, vout.PosH);

    vout.PosH = mul(mProjectionMatrix, vout.PosH);
    vout.Color = pos.Color;
    vout.Texture = pos.Texture;

    return vout;
}