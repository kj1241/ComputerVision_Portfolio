using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaterManger : MonoBehaviour
{
    public Material WaveMaterial;
    public ComputeShader WaveComputers; //��ǻƮ ���̴� ����
    public RenderTexture curState, beforState, afterState; //���� �ý�ó�� ���� ������ �������, ������, �������� ����
    public Vector2Int resolution = new Vector2Int(256, 256); // UV ���
    public RenderTexture obstaclesTex; //��ֹ�

    public Vector3 effect; //x ,y, ��
    public float dispersion = 0.98f; // �л� �߰� �ĵ��� �󸶳� ������ �ŷڵ� 95% ����

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

        WaveComputers.Dispatch(0, resolution.x / 8, resolution.y / 8, 1); //�̰� GPU ��ǻƮ ���̴����� ������ xyz ������ ����
    }

   
    void InitillizeTexture(ref RenderTexture tex)
    {//3���� ���� ���� 4���� ���� rgba float �������� �����Ǵ� 64ä��
        tex = new RenderTexture(resolution.x, resolution.y, 1, UnityEngine.Experimental.Rendering.GraphicsFormat.R16G16B16A16_SNorm);
        tex.enableRandomWrite = true; //���Ⱑ��
        tex.Create(); // ����
    }
}
