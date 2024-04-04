Shader "Unlit/ParticleShader"
{
	Properties
	{
		[HDR] _Color ("Tint", Color) = (0, 0, 0, 1)
	}
	SubShader
	{
		Pass
		{
			Tags{ "RenderType"="Opaque" "Queue"="Geometry" }

			CGPROGRAM
			#include "UnityCG.cginc"

			#pragma vertex vert //버텍스
			#pragma fragment frag //픽셀 surface 안쓰는이유 표면쉐이더로 그림자 , 빛반사 사용할꺼아니라서

			fixed4 _Color;
			
			//선언된 버퍼
			StructuredBuffer<float3> SphereLocations;
			StructuredBuffer<int> Triangles;
			StructuredBuffer<float3> Positions;

			//버텍스 쉐이더
			float4 vert(uint vertex_id: SV_VertexID, uint instance_id: SV_InstanceID) : SV_POSITION
			{
				int positionIndex = Triangles[vertex_id]; //포지션 인덱스 삼각형 인덱스
				float3 position = Positions[positionIndex]; //각 쓰래드에 있는 파티클 개인 위치
				position += SphereLocations[instance_id]; //개별위치 정해주기
				//월드 위치를 클리핑 위치로 해줌
				return mul(UNITY_MATRIX_VP, float4(position, 1)); //UNITY_MATRIX_VP = 프로젝션 매트릭스.
			}

			//픽쉘 세이더
			fixed4 frag() : SV_TARGET
			{
				return _Color;
			}
			ENDCG
		}
	}
	Fallback "VertexLit"
}
