#pragma once
#ifdef CREATING_DLL //definido en el .cpp

#define MYDLL_API_HELPER __declspec(dllexport)//crear dll

#else

#define MYDLL_API_HELPER __declspec(dllimport)//crear dll

#endif // CREATING_DLL


// These are some simple math helpers to enable the template to render a spinning cube. It is not a complete math library.
// You can replace this with your favorite math library that's suitable for your target platforms, e.g. DirectXMath or GLM.

#include <math.h>
#include "CVector3DLL.h"

namespace MathHelperDLL
{
	struct MYDLL_API_HELPER Matrix4DLL
	{
		Matrix4DLL(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}

		float m[4][4];
	};

	inline static Matrix4DLL IdentityMatrix()
	{
		return Matrix4DLL(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	// Rotate around Y
	inline static Matrix4DLL SimpleModelMatrix(float angleInRadians)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);

		return Matrix4DLL(cosine, 0.0f, -sine, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sine, 0.0f, cosine, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static Matrix4DLL MatrixMult(Matrix4DLL m1, Matrix4DLL m2)
	{
		float r[4][4];
		float mult = 0.0f;
		int t = 0;
		for (int i = 0; i < 4; i++)
		{
			mult = 0.0f;
			for (int j = 0; j < 4; j++)
			{
				mult += m1.m[i][j] * m2.m[j][i];
			}

			r[i][t] = mult;
			t++;

		}

		return Matrix4DLL(
			r[0][0], r[0][1], r[0][2], r[0][3],
			r[1][0], r[1][1], r[1][2], r[1][3],
			r[2][0], r[2][1], r[2][2], r[2][3],
			r[3][0], r[3][1], r[3][2], r[3][3]
		);
	}

	// Rotate around Y + Translate
	inline static Matrix4DLL ModelMatrix(float angleInRadians, CVector3DLL translation)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);

		// NOTE: Remember this needs to be a transpose of the rotation matrix
		return Matrix4DLL(cosine, 0.0f, -sine, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sine, 0.0f, cosine, 0.0f,
			translation.getX(), translation.getY(), translation.getZ(), 1.0f);
	}

	//
	inline static Matrix4DLL Multiply(Matrix4DLL m1, Matrix4DLL m2)
	{
		// TO-DO
		return IdentityMatrix();
	}

	// Return a View Matrix given the Camera vectors
	inline static Matrix4DLL ViewMatrix(CVector3DLL camEyePos, CVector3DLL camLookAt, CVector3DLL camUpVec)
	{
		// Calculate forward vector from target to eye
		CVector3DLL forward = camEyePos - camLookAt;
		forward.normalize();

		// Calculate left vector
		CVector3DLL left = camUpVec.cross(forward);
		left.normalize();

		// Recalculate the orthonormal up vector
		CVector3DLL up = forward.cross(left);

		// Calculate inverse translation from camEyePos to camLookAt
		float inverseTranslateX = -(left.X    * camEyePos.X) - (left.Y    * camEyePos.Y) - (left.Z    * camEyePos.Z);
		float inverseTranslateY = -(up.X      * camEyePos.X) - (up.Y      * camEyePos.Y) - (up.Z      * camEyePos.Z);
		float inverseTranslateZ = -(forward.X * camEyePos.X) - (forward.Y * camEyePos.Y) - (forward.Z * camEyePos.Z);

		return Matrix4DLL(left.X, left.Y, left.Z, inverseTranslateX,
			up.X, up.Y, up.Z, inverseTranslateY,
			forward.X, forward.Y, forward.Z, inverseTranslateZ,
			0.0f, 0.0f, 0.0f, 1.0f);

		// Transpose??
	}

	inline static Matrix4DLL SimpleViewMatrix(float cameraDistance)
	{
		// Camera is at 60 degrees to the ground, in the YZ plane.
		// Camera Look-At is hardcoded to (0, 0, 0).
		// Camera Up is hardcoded to (0, 1, 0).
		const float sqrt3over2 = 0.86603f;

		return Matrix4DLL(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, sqrt3over2, 0.5f, 0.0f,
			0.0f, -0.5f, sqrt3over2, 0.0f,
			0.0f, 0.0f, -cameraDistance, 1.0f);
	}

	inline static Matrix4DLL SimpleProjectionMatrix(float aspectRatio)
	{
		// Far plane is at 50.0f, near plane is at 1.0f.
		// FoV is hardcoded to pi/3.
		const float cotangent = 1 / tanf(3.14159f / 6.0f);

		return Matrix4DLL(cotangent / aspectRatio, 0.0f, 0.0f, 0.0f,
			0.0f, cotangent, 0.0f, 0.0f,
			0.0f, 0.0f, -50.0f / (50.0f - 1.0f), (-50.0f * 1.0f) / (50.0f - 1.0f),
			0.0f, 0.0f, -1.0f, 0.0f);
	}
}