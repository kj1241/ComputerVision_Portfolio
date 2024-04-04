using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TextureFilterExample : MonoBehaviour
{
    public ComputeShader filterComputeShader;
    public Texture2D inputTexture;
    public Material outputMaterial;

    void Start()
    {
        ApplyFilter();
    }

    void ApplyFilter()
    {
        int width = inputTexture.width;
        int height = inputTexture.height;

        // 결과에 대한 RenderTexture 생성
        RenderTexture resultTexture = new RenderTexture(width, height, 0);
        resultTexture.enableRandomWrite = true;
        resultTexture.Create();

        // 컴퓨팅 셰이더 매개변수 설정
        filterComputeShader.SetTexture(0, "InputTexture", inputTexture);
        filterComputeShader.SetTexture(0, "ResultTexture", resultTexture);

        // 컴퓨팅 셰이더 디스패치
        int threadGroupsX = Mathf.CeilToInt(width / 8.0f);
        int threadGroupsY = Mathf.CeilToInt(height / 8.0f);
        filterComputeShader.Dispatch(0, threadGroupsX, threadGroupsY, 1);

        // 결과를 머티리얼이나 다른 GameObject에 적용
        outputMaterial.mainTexture = resultTexture;
    }
}
