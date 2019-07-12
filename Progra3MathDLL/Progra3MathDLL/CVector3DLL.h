#pragma once
#ifdef CREATING_DLL //definido en el .cpp

#define MYDLL_API_CLASS __declspec(dllexport)//crear dll

#else

#define MYDLL_API_CLASS __declspec(dllimport)//crear dll

#endif // CREATING_DLL

#ifndef CVECTOR3DLL_H
#define CVECTOR3DLL_H

// Simple class to practice C++ class creation
// It will be more helpful later when more code is added
class MYDLL_API_CLASS CVector3DLL
{
public:
	float X, Y, Z;

public:
	CVector3DLL();
	CVector3DLL(float _x, float _y, float _z);
	CVector3DLL(const CVector3DLL &other);
	~CVector3DLL();

	CVector3DLL& operator=(const CVector3DLL &other);

	CVector3DLL operator+(const CVector3DLL & other);
	CVector3DLL& operator+=(const CVector3DLL &other);

	CVector3DLL operator-(const CVector3DLL & other);
	CVector3DLL& operator-=(const CVector3DLL &other);

	CVector3DLL cross(const CVector3DLL & other);
	float dot(const CVector3DLL & other);

	void setValues(float _x, float _y, float _z);
	void setValues(float *values);
	float getX() const { return X; }
	float getY() const { return Y; }
	float getZ() const { return Z; }
	void getValues(float *values) const { values[0] = X; values[1] = Y; values[2] = Z; }
	void normalize();
	float magnitude();

	inline static CVector3DLL ZeroVector()
	{
		return CVector3DLL(0.0f, 0.0f, 0.0f);
	}

private:
	float quickReverseSqrt(float number);
	float quickReverseSqrt2(float number);
};

#endif // !CVECTOR3_H
