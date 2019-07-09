#pragma once
class CObjectInstance
{
public:
	CObjectInstance();
	CObjectInstance(int objIndex, float scale);
	~CObjectInstance();
	int m_objectIndex;
	int m_textureIndex;
	float m_position;
	float m_scale;
	float m_rotation;
};

