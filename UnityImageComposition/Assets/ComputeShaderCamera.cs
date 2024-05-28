using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class ComputeShaderCamera : MonoBehaviour
{

    public Texture2D sourceTexture; // 보여줄 텍스쳐
    public Material blitMaterial; // 텍스쳐를 그릴 재질

    private Camera renderCamera; // 렌더링을 위한 카메라
    private RenderTexture renderTexture; // 렌더링 텍스쳐

    public ComputeShader filterComputeShader;
    RenderTexture resultTexture;
    public float weight;
    // Start is called before the first frame update
    void Start()
    {
        
        renderCamera = gameObject.AddComponent<Camera>();
        renderCamera.clearFlags = CameraClearFlags.SolidColor;
        renderCamera.backgroundColor = Color.black;
        renderCamera.orthographic = true;
        renderCamera.nearClipPlane = 0.1f;
        renderCamera.farClipPlane = 1000f;
        renderCamera.depth = -100; // 다른 요소들 위에 표시하기 위해 깊이 설정

        // 렌더링 텍스쳐 생성
        renderTexture = new RenderTexture(Screen.width, Screen.height, 0);
        renderTexture.enableRandomWrite = true;
        renderTexture.Create();

        renderCamera.targetTexture = renderTexture;
     
        resultTexture = new RenderTexture(Screen.width, Screen.height, 0);
        resultTexture.enableRandomWrite = true;
        resultTexture.Create();


    }

    // Update is called once per frame
    void Update()
    {
        // 렌더링 텍스쳐에 텍스쳐를 그리기 위해 설정
        Graphics.SetRenderTarget(renderTexture);

        // 텍스쳐를 그릴 재질과 텍스쳐를 사용하여 Graphics.Blit() 호출
        //Graphics.Blit(sourceTexture, renderTexture, blitMaterial);

        ApplyFilter();

    }
    void OnGUI()
    {
        // 게임뷰 화면에 렌더링 텍스쳐를 표시
        GUI.DrawTexture(new Rect(0, 0, Screen.width, Screen.height), resultTexture);
    }
    //void OnRenderImage(RenderTexture src, RenderTexture dest)
    //{
    //    // 게임뷰에 렌더링 텍스쳐를 표시
    //    Graphics.Blit(renderTexture, dest);
    //}

    void ApplyFilter()
    {
        // 컴퓨팅 셰이더 매개변수 설정
        filterComputeShader.SetTexture(0, "InputTexture", renderTexture);
        filterComputeShader.SetTexture(0, "ResultTexture", resultTexture);
        filterComputeShader.SetFloat("weight", weight);

        // 컴퓨팅 셰이더 디스패치
        int threadGroupsX = Mathf.CeilToInt(renderTexture.width / 8.0f);
        int threadGroupsY = Mathf.CeilToInt(renderTexture.height / 8.0f);
        filterComputeShader.Dispatch(0, threadGroupsX, threadGroupsY, 1);

    }
}

[CustomEditor(typeof(ComputeShaderCamera))] 
public class ComputeShaderCameraEditor : Editor
{
    public override void OnInspectorGUI()
    {
        ComputeShaderCamera Script = (ComputeShaderCamera)target;

        EditorGUI.BeginChangeCheck();
        float value = EditorGUILayout.Slider("Value", Script.weight, 0f, 1f);
        if (EditorGUI.EndChangeCheck())
        {
            Script.weight = value;
        }
    }
}