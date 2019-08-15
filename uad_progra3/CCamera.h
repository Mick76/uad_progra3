#pragma once
#include "../CVector3.h"
#include "../MathHelper.h"
#include "CFrustum.h"

class CCamera
{
private:
public:
	MathHelper::Matrix4 m_projMatrix, //calcular en el c for
						m_viewMatrix; //recalcular cada vez que se mueva la camara
	CVector3 m_pos, m_rightVec, m_upVec, m_lookAt;
	CFrustum m_frustum;

	bool isVolumeVisible(AABB box);

	CCamera();
	~CCamera();

};

