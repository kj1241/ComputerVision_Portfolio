using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public class FlowFieldManger : MonoBehaviour
{
    public int SphereAmount = 100;
    public ComputeShader Shader;

    public Mesh Mesh;  //�޽�
    public Material Material; //���͸���
    public float Scale = 1; //Ȯ��

    ComputeBuffer resultBuffer; //��� ����
    ComputeBuffer meshTriangles; //�޽� Ʈ���� ����
    ComputeBuffer meshPositions; //�޽� ��ġ

    int kernel; //Ŀ��
    uint threadGroupSize; //������ �׷�
    int threadGroups; //������ �׷�
    Bounds bounds; //�ٿ�� �ڽ�

    // Start is called before the first frame update
    void Start()
    {
        kernel = Shader.FindKernel("Spheres"); //mainCS ��� ���� Ŀ�� ���� �ؼ� ����
        Shader.GetKernelThreadGroupSizes(kernel, out threadGroupSize, out _, out _); //GPU �����带 �󸶸�ŭ ũ��� ������ �ʴ� hlsl���� �޾ƿ���
        threadGroups = (int)((SphereAmount + (threadGroupSize - 1)) / threadGroupSize);

        resultBuffer = new ComputeBuffer(SphereAmount, sizeof(float) * 3);   //��ü ����� ���� ��ġ ����

        int[] triangles = Mesh.triangles; //�޽��� ���� �� ����� ������ ��ο� ���� �ɵ�
        meshTriangles = new ComputeBuffer(triangles.Length, sizeof(int)); // ũ�⸸ŭ ���� �����Ҵ�
        meshTriangles.SetData(triangles); //������ �ѱ��

        Vector3[] positions = Mesh.vertices.Select(p => p * Scale).ToArray(); //���ý� ũ���� ���Ͱ��� ��Į�� ���� ������
        meshPositions = new ComputeBuffer(positions.Length, sizeof(float) * 3); //������ ũ�⸦ ���۷� �ѱ�
        meshPositions.SetData(positions);

        Shader.SetBuffer(kernel, "Result", resultBuffer);

        Material.SetBuffer("SphereLocations", resultBuffer);
        Material.SetBuffer("Triangles", meshTriangles);
        Material.SetBuffer("Positions", meshPositions);

        bounds = new Bounds(Vector3.zero, Vector3.one );
    }

    // Update is called once per frame
    void Update()
    {
        Shader.SetFloat("Time", Time.time); //��Ÿ Ÿ�� �ð���ȭ�� �Ѱ��ֱ����ؼ�
        Shader.Dispatch(kernel, threadGroups, 1, 1); //Ŀ�ο� ������ ����

        Graphics.DrawProcedural(Material, bounds, MeshTopology.Triangles, meshTriangles.count, SphereAmount);  //��� �׷���
    }

    //���� ����
    void OnDestroy()
    {
        resultBuffer.Dispose();
        meshTriangles.Dispose();
        meshPositions.Dispose();
    }
}
