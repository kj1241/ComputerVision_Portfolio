using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestBox : MonoBehaviour
{
    [Range(1, 1000000)]
    public int instanceCount = 100000; //����
    public Mesh mesh; //�׷��� ����
    public Material material; //���͸���
    public int subMeshIndex = 0; //�ε���
    public Bounds renderBounds = new Bounds(Vector3.zero, Vector3.one * 50f);

    private ComputeBuffer argsBuffer;     // �޽� ������ ����
    private ComputeBuffer positionBuffer; // ��ġ ������ ����
    private uint[] argsData = new uint[5];

    // ������� ����
    private int cachedInstanceCount;
    private int cachedSubMeshIndex;
    private void Update()
    {
        if (mesh == null || material == null)
            return;

        // ������� ���� ��� ���� ������ϱ�
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

    // �޽� ������ ���� ����
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

    //��ġ, ������ ������ ���� ����
    private void InitPositionBuffer()
    {
        if (positionBuffer != null)
            positionBuffer.Release();

        Vector4[] positions = new Vector4[instanceCount];
        Vector3 boundsMin = renderBounds.min;
        Vector3 boundsMax = renderBounds.max;

        // XYZ : ��ġ, W : ������
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
            mesh,         // �׷��� �޽�
            subMeshIndex, // ����޽� �ε���
            material,     // �׷��� ���׸���
            renderBounds, // ������ ����
            argsBuffer    // �޽� ������ ����
        );
    }
}
