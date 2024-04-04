Shader "Custom/EmbossingShader"
{
    Properties
    {
        _MainTex("Base (RGB)", 2D) = "white" { }
        _Intensity("Emboss Intensity", Range(0.1, 1.0)) = 0.5
    }

        CGINCLUDE
#include "UnityCG.cginc"

            struct appdata
        {
            float4 vertex : POSITION;
            float3 normal : NORMAL;
        };

        struct v2f
        {
            float4 pos : POSITION;
            float4 color : COLOR;
        };
        ENDCG

            SubShader
        {
            Tags {"Queue" = "Overlay" }
            LOD 100

            Pass
            {
                CGPROGRAM
                #pragma vertex vert
                #pragma fragment frag

                // 전역 변수
                uniform sampler2D _MainTex;
                uniform float _Intensity;

                // 버텍스 프로그램
                v2f vert(appdata v)
                {
                    v2f o;
                    o.pos = UnityObjectToClipPos(v.vertex);
                    o.color = float4(1,1,1,1); // 기본 색상은 흰색
                    return o;
                }

                // 픽셀 프로그램
                fixed4 frag(v2f i) : COLOR
                {
                    // 노말을 텍스처 좌표로 변환
                    float2 normalMap = (i.color.xy * 0.5) + 0.5;

                    // 텍스처에서 노말 값을 가져옴
                    fixed3 normal = tex2D(_MainTex, normalMap).rgb;

                    // Emboss 효과 계산
                    float emboss = dot(normal, float3(1, 1, -1));
                    emboss = (emboss + 1) * 0.5; // Emboss 값을 0에서 1로 매핑

                    // Intensity를 적용하여 픽셀 색상을 조절
                    fixed3 col = lerp(fixed3(0, 0, 0), fixed3(1, 1, 1), _Intensity);

                    // 최종 색상 계산
                    fixed4 finalColor = tex2D(_MainTex, normalMap);
                    finalColor.rgb += emboss * col;

                    return finalColor;
                }
                ENDCG
            }
        }
}
