using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public class FlowFieldManger : MonoBehaviour
{
    public int SphereAmount = 100;
    public ComputeShader Shader;

    public Mesh Mesh;  //메쉬
    public Material Material; //머터리얼
    public float Scale = 1; //확대

    ComputeBuffer resultBuffer; //결과 버퍼
    ComputeBuffer meshTriangles; //메쉬 트라이 엥글
    ComputeBuffer meshPositions; //메쉬 위치

    int kernel; //커널
    uint threadGroupSize; //쓰레드 그룹
    int threadGroups; //쓰레드 그룹
    Bounds bounds; //바운드 박스

    // Start is called before the first frame update
    void Start()
    {
        kernel = Shader.FindKernel("Spheres"); //mainCS 대신 관련 커널 정의 해서 쓰기
        Shader.GetKernelThreadGroupSizes(kernel, out threadGroupSize, out _, out _); //GPU 쓰레드를 얼마만큼 크기로 쓸껀지 ㅚ니 hlsl에서 받아오기
        threadGroups = (int)((SphereAmount + (threadGroupSize - 1)) / threadGroupSize);

        resultBuffer = new ComputeBuffer(SphereAmount, sizeof(float) * 3);   //구체 만들기 위한 위치 버퍼

        int[] triangles = Mesh.triangles; //메쉬의 갯수 이 사이즈가 실질적 드로우 콜이 될듯
        meshTriangles = new ComputeBuffer(triangles.Length, sizeof(int)); // 크기만큼 버퍼 동적할당
        meshTriangles.SetData(triangles); //데이터 넘기기

        Vector3[] positions = Mesh.vertices.Select(p => p * Scale).ToArray(); //버택스 크기의 백터값을 스칼라 값과 곱해줌
        meshPositions = new ComputeBuffer(positions.Length, sizeof(float) * 3); //실질적 크기를 버퍼로 넘김
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
        Shader.SetFloat("Time", Time.time); //델타 타임 시간변화량 넘겨주기위해서
        Shader.Dispatch(kernel, threadGroups, 1, 1); //커널에 쓰래드 적용

        Graphics.DrawProcedural(Material, bounds, MeshTopology.Triangles, meshTriangles.count, SphereAmount);  //결과 그려줌
    }

    //버퍼 해제
    void OnDestroy()
    {
        resultBuffer.Dispose();
        meshTriangles.Dispose();
        meshPositions.Dispose();
    }
}
