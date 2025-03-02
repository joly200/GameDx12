// Ŀ�������ɫ���汾
#pragma target cs_5_0

// �������������
RWTexture2D<float4> InputBuffer : register(u0);
RWTexture2D<float4> OutputBuffer : register(u1);

#define NUM_SAMPLES 7  // ȷ�������Сһ��
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

    // FXAA�������
    float2 RcpTextureSize = 1.0 / resolution;

    // ��ȡ��ǰ��������
    float currentLuma = dot(pixel0.rgb, float3(0.299, 0.587, 0.114)); // ʹ��YUVת��������

    // �������λ�ú;���
    float2 StartUV = currentPixelCoord * RcpTextureSize;
    float4 CenterColor = pixel0; // ��ǰ������ɫ

    // ��ʼ������
    float NegDist = s_SampleDistances[NUM_SAMPLES];
    float PosDist = s_SampleDistances[NUM_SAMPLES];
    bool NegGood = false;
    bool PosGood = false;

    // ���Һ��ʵĸ����������������
    for (uint iter = 0; iter < NUM_SAMPLES; ++iter)
    {
        const float Distance = s_SampleDistances[iter];

        // �������������Ĳ���UV����
        float2 NegUV = StartUV - float2(0, RcpTextureSize.y) * Distance;
        float2 PosUV = StartUV + float2(0, RcpTextureSize.y) * Distance;

        // ����������
        float4 NegColor = InputBuffer.Load(int3(NegUV * resolution, 0));
        float NegLuma = dot(NegColor.rgb, float3(0.299, 0.587, 0.114)); // ��ȡ�����������
        if (abs(NegLuma - currentLuma) >= 0.25 && Distance < NegDist)
        {
            NegDist = Distance;
            NegGood = (sign(NegLuma - currentLuma) == sign(currentLuma - CenterColor.a)); // ȷ����Ե����
        }

        // ����������
        float4 PosColor = InputBuffer.Load(int3(PosUV * resolution, 0));
        float PosLuma = dot(PosColor.rgb, float3(0.299, 0.587, 0.114)); // ��ȡ�����������
        if (abs(PosLuma - currentLuma) >= 0.25 && Distance < PosDist)
        {
            PosDist = Distance;
            PosGood = (sign(PosLuma - currentLuma) == sign(currentLuma - CenterColor.a)); // ȷ����Ե����
        }
    }

    // ����ƽ������
    float PixelShift = 0.5 - min(NegDist, PosDist) / (PosDist + NegDist);
    PixelShift = max(0.0, PixelShift); // ������ָ�ֵ

    // �����������ֵ�����������
    if (PixelShift > 0.0)
    {
        // ��ֵӦ��FXAA����
        OutputBuffer[currentPixelCoord] = lerp(CenterColor, InputBuffer[currentPixelCoord], PixelShift);


    }
    else
    {
        // ����ֱ�������ǰ����
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

    
