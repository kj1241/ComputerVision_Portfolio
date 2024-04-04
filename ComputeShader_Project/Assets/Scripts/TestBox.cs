using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestBox : MonoBehaviour
{
    [Range(1, 1000000)]
    public int instanceCount = 100000; //갯수
    public Mesh mesh; //그려줄 형태
    public Material material; //머터리얼
    public int subMeshIndex = 0; //인덱스
    public Bounds renderBounds = new Bounds(Vector3.zero, Vector3.one * 50f);

    private ComputeBuffer argsBuffer;     // 메시 데이터 버퍼
    private ComputeBuffer positionBuffer; // 위치 스케일 버퍼
    private uint[] argsData = new uint[5];

    // 변경사항 감지
    private int cachedInstanceCount;
    private int cachedSubMeshIndex;
    private void Update()
    {
        if (mesh == null || material == null)
            return;

        // 변경사항 생길 경우 버퍼 재생성하기
        if (cachedInstanceCount != instanceCount || cachedSubMeshIndex != subMeshIndex)
        {
            InitArgsBuffer();
            InitPositionBuffer();

            cachedInstanceCount = instanceCount;
            cachedSubMeshIndex = subMeshIndex;
        }

        DrawInstances();
    }

    private void OnDestroy()
    {
        if (argsBuffer != null)
            argsBuffer.Release();

        if (positionBuffer != null)
            positionBuffer.Release();
    }

    // 메시 데이터 버퍼 생성
    private void InitArgsBuffer()
    {
        if (argsBuffer == null)
            argsBuffer = new ComputeBuffer(1, sizeof(uint) * 5, ComputeBufferType.IndirectArguments);

        argsData[0] = (uint)mesh.GetIndexCount(subMeshIndex);
        argsData[1] = (uint)instanceCount;
        argsData[2] = (uint)mesh.GetIndexStart(subMeshIndex);
        argsData[3] = (uint)mesh.GetBaseVertex(subMeshIndex);
        argsData[4] = 0;

        argsBuffer.SetData(argsData);
    }

    //위치, 스케일 데이터 버퍼 생성
    private void InitPositionBuffer()
    {
        if (positionBuffer != null)
            positionBuffer.Release();

        Vector4[] positions = new Vector4[instanceCount];
        Vector3 boundsMin = renderBounds.min;
        Vector3 boundsMax = renderBounds.max;

        // XYZ : 위치, W : 스케일
        for (int i = 0; i < instanceCount; i++)
        {
            ref Vector4 pos = ref positions[i];
            pos.x = UnityEngine.Random.Range(boundsMin.x, boundsMax.x);
            pos.y = UnityEngine.Random.Range(boundsMin.y, boundsMax.y);
            pos.z = UnityEngine.Random.Range(boundsMin.z, boundsMax.z);
            pos.w = UnityEngine.Random.Range(0.25f, 1f); 
        }

        positionBuffer = new ComputeBuffer(instanceCount, sizeof(float) * 4);
        positionBuffer.SetData(positions);

        material.SetBuffer("positionBuffer", positionBuffer);
    }

    private void DrawInstances()
    {
        Graphics.DrawMeshInstancedIndirect(
            mesh,         // 그려낼 메시
            subMeshIndex, // 서브메시 인덱스
            material,     // 그려낼 마테리얼
            renderBounds, // 렌더링 영역
            argsBuffer    // 메시 데이터 버퍼
        );
    }
}
