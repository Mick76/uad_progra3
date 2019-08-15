#include "CCamera.h"



bool CCamera::isVolumeVisible(AABB box)
{
	if (m_frustum.isAABBVisible(box) == CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE || m_frustum.isAABBVisible(box) == CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT)
	{
		return true;
	}
	return false;
}

CCamera::CCamera()
{
	m_pos = CVector3::ZeroVector();
	m_lookAt = CVector3::ZeroVector();
	m_upVec = CVector3::ZeroVector();
	m_rightVec = CVector3::ZeroVector();
}


CCamera::~CCamera()
{
}
