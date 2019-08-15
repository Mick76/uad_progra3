#include "AABB.h"



AABB::AABB()
{
}


AABB::~AABB()
{
}

void AABB::setPoints(CVector3 points[4])
{
	m_points[0] = points[0];
	m_points[1] = points[1];
	m_points[2] = points[2];
	m_points[3] = points[3];
}
