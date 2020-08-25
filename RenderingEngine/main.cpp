#include <iostream>

#include "primitives.h"

void PrintMatrix(const Matrix4x4& m)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			printf("%.3f ", m.m[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	Matrix4x4 m1;
	m1.m[0][0] = 123;
	m1.m[0][1] = 33;
	m1.m[1][3] = 44;
	m1.m[3][3] = 83;

	Matrix4x4 m2;

	m2.m[0][0] = 123;
	m2.m[0][1] = 33;
	m2.m[1][3] = 44;
	m2.m[3][3] = 83;

	auto m3 = m1 * m2;
	//PrintMatrix(m3);

	Transform t(Vector3(1, 0, 0), Rotator(90, 0, 0), Vector3(2.f, 2.f, 2.f));
	PrintMatrix(t.ToMatrix());


	return 0;
}