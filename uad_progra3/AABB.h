#pragma once
#include "../CVector3.h"
class AABB
{
private:


public:
	AABB();
	~AABB();

	CVector3 m_points[4];
	void setPoints(CVector3 points[4]);
};

