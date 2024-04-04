#ifndef WHITE_NOISE
#define WHITE_NOISE
float random(float value, float mutation = 0.546)
{
	float random = frac(sin(value + mutation) * 141421.3562);  //* 141421.3562);
	return random;
}

float3 random3d(float value) {
	return float3(
		random(value, 2.5),
		random(value, 5.0),
		random(value, 7.5)
		);
}

#endif