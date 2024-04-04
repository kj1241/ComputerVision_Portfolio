using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaterManger : MonoBehaviour
{
    public Material WaveMaterial;
    public ComputeShader WaveComputers; //컴퓨트 쉐이더 정의
    public RenderTexture curState, beforState, afterState; //렌더 택스처의 종료 시점의 현재상태, 전상태, 앞으로의 상태
    public Vector2Int resolution = new Vector2Int(256, 256); // UV 결과
    public RenderTexture obstaclesTex; //장애물

    public Vector3 effect; //x ,y, 양
    public float dispersion = 0.98f; // 분산 추가 파도가 얼마나 많은지 신뢰도 95% 였나

    void Start()
    {
        InitillizeTexture(ref curState);
        InitillizeTexture(ref beforState);
        InitillizeTexture(ref afterState);
        obstaclesTex.enableRandomWrite = true;
        WaveMaterial.mainTexture = curState;
        //Debug.Assert(obstaclesTex.width == resolution.x && obstaclesTex.height == resolution.y);
    }

    void Update()
    {
        Graphics.CopyTexture(curState, beforState);
        Graphics.CopyTexture(afterState, curState);


        WaveComputers.SetTexture(0, "curState", curState);
        WaveComputers.SetTexture(0, "beforState", beforState);
        WaveComputers.SetTexture(0, "afterState", afterState);
        WaveComputers.SetVector("effect", effect);
        WaveComputers.SetVector("resolution", new Vector2(resolution.x, resolution.y));
        WaveComputers.SetFloat("dispersion", dispersion);
        WaveComputers.SetTexture(0, "obstaclesTex", obstaclesTex);

        WaveComputers.Dispatch(0, resolution.x / 8, resolution.y / 8, 1); //이건 GPU 컴퓨트 쉐이더에서 선언한 xyz 쓰래드 갯수
    }

   
    void InitillizeTexture(ref RenderTexture tex)
    {//3번쨰 인자 깊이 4번쨰 인자 rgba float 형식으로 지정되는 64채널
        tex = new RenderTexture(resolution.x, resolution.y, 1, UnityEngine.Experimental.Rendering.GraphicsFormat.R16G16B16A16_SNorm);
        tex.enableRandomWrite = true; //쓰기가능
        tex.Create(); // 생성
    }
}
