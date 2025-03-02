// 目标计算着色器版本
#pragma target cs_5_0

// 输入输出缓冲区
RWTexture2D<float4> InputBuffer : register(u0);
RWTexture2D<float4> OutputBuffer : register(u1);

#define NUM_SAMPLES 7  // 确保数组大小一致
static const float s_SampleDistances[10] =
{
    1.0, 2.5, 4.5, 6.5, 8.5, 10.5, 14.5, 22.5, 36.5, 48.5
};

[numthreads(32, 32, 1)]
void CSMain(uint3 threadID : SV_DispatchThreadID)
{
    int2 currentPixelCoord = threadID.xy;

    float size = 15;
    float4 pixel0 = InputBuffer[currentPixelCoord];
    float2 resolution = float2(768, 432);

    // FXAA处理参数
    float2 RcpTextureSize = 1.0 / resolution;

    // 获取当前像素亮度
    float currentLuma = dot(pixel0.rgb, float3(0.299, 0.587, 0.114)); // 使用YUV转换的亮度

    // 定义采样位置和距离
    float2 StartUV = currentPixelCoord * RcpTextureSize;
    float4 CenterColor = pixel0; // 当前像素颜色

    // 初始化变量
    float NegDist = s_SampleDistances[NUM_SAMPLES];
    float PosDist = s_SampleDistances[NUM_SAMPLES];
    bool NegGood = false;
    bool PosGood = false;

    // 查找合适的负采样点和正采样点
    for (uint iter = 0; iter < NUM_SAMPLES; ++iter)
    {
        const float Distance = s_SampleDistances[iter];

        // 负方向和正方向的采样UV坐标
        float2 NegUV = StartUV - float2(0, RcpTextureSize.y) * Distance;
        float2 PosUV = StartUV + float2(0, RcpTextureSize.y) * Distance;

        // 采样负方向
        float4 NegColor = InputBuffer.Load(int3(NegUV * resolution, 0));
        float NegLuma = dot(NegColor.rgb, float3(0.299, 0.587, 0.114)); // 获取负方向的亮度
        if (abs(NegLuma - currentLuma) >= 0.25 && Distance < NegDist)
        {
            NegDist = Distance;
            NegGood = (sign(NegLuma - currentLuma) == sign(currentLuma - CenterColor.a)); // 确定边缘方向
        }

        // 采样正方向
        float4 PosColor = InputBuffer.Load(int3(PosUV * resolution, 0));
        float PosLuma = dot(PosColor.rgb, float3(0.299, 0.587, 0.114)); // 获取正方向的亮度
        if (abs(PosLuma - currentLuma) >= 0.25 && Distance < PosDist)
        {
            PosDist = Distance;
            PosGood = (sign(PosLuma - currentLuma) == sign(currentLuma - CenterColor.a)); // 确定边缘方向
        }
    }

    // 计算平移因子
    float PixelShift = 0.5 - min(NegDist, PosDist) / (PosDist + NegDist);
    PixelShift = max(0.0, PixelShift); // 避免出现负值

    // 将采样结果插值到输出缓冲区
    if (PixelShift > 0.0)
    {
        // 插值应用FXAA处理
        OutputBuffer[currentPixelCoord] = lerp(CenterColor, InputBuffer[currentPixelCoord], PixelShift);


    }
    else
    {
        // 否则直接输出当前像素
        OutputBuffer[currentPixelCoord] = pixel0;
    }
    if ((currentPixelCoord.y < resolution.y + size) && (currentPixelCoord.y > resolution.y - size) && (currentPixelCoord.x == 768))
    {
        OutputBuffer[currentPixelCoord] = float4(1, 0, 0, 1);
        return;
    }
    if ((currentPixelCoord.x < resolution.x + size) && (currentPixelCoord.x > resolution.x - size) && (currentPixelCoord.y == 432))
    {
        OutputBuffer[currentPixelCoord] = float4(5, 0, 0, 1);
        return;
    }
}

    
