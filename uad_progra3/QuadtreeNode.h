#pragma once
#include "AABB.h"
#include "CCamera.h"
#include <vector>

using namespace std;

template <class T>
class QuadtreeNode
{
private:
	QuadtreeNode<T> *m_n1, *m_n2, *m_n3, *m_n4;
	vector<T*> m_data;	//ptrs a celdas
	AABB m_boundingBox;

public:
	QuadtreeNode();
	~QuadtreeNode();

	void setAABB(AABB box);
	void render(CCamera *cam, vector<T*> *visibleObjects);
	void subDivide(int limit, vector<T*> grid);
};



template <class T>
QuadtreeNode<T>::QuadtreeNode()
{
	m_n1 = nullptr;
	m_n2 = nullptr;
	m_n3 = nullptr;
	m_n4 = nullptr;
}

template <class T>
QuadtreeNode<T>::~QuadtreeNode()
{
}

template<class T>
inline void QuadtreeNode<T>::setAABB(AABB box)
{
	m_boundingBox = box;
}

template<class T>
inline void QuadtreeNode<T>::render(CCamera * cam, vector<T*>* visibleObjects)
{
	if (cam->isVolumeVisible(m_boundingBox))
	{
		visibleObjects->insert(visibleObjects->end(), m_data.begin(), m_data.end());
	}
	if (m_n1 != nullptr)
	{
		m_n1->render(cam, visibleObjects);
	}

	if (m_n2 != nullptr)
	{
		m_n2->render(cam, visibleObjects);
	}

	if (m_n3 != nullptr)
	{
		m_n3->render(cam, visibleObjects);
	}

	if (m_n4 != nullptr)
	{
		m_n4->render(cam, visibleObjects);
	}
}

template<class T>
inline void QuadtreeNode<T>::subDivide(int limit, vector<T*> grid)
{
	int  count = 0;
	//para cada elemento del vector verificar si esta dentro del bounding box e incrementar contador
	for (int i = 0; i < grid.size(); i++)
	{
		if (m_boundingBox.m_points[0].X <= grid[i]->m_cellPosition.X && grid[i]->m_cellPosition.X <= m_boundingBox.m_points[3].X && m_boundingBox.m_points[0].Z <= grid[i]->m_cellPosition.Z && grid[i]->m_cellPosition.Z <= m_boundingBox.m_points[3].Z)
		{
			// La celda esta dentro del bounding box
			count++;
			m_data.push_back(grid[i]);
		}
	}

	if (count > limit)
	{
		m_n1 = new QuadtreeNode<T>();
		m_n2 = new QuadtreeNode<T>();
		m_n3 = new QuadtreeNode<T>();
		m_n4 = new QuadtreeNode<T>();

		AABB aabb1;
		CVector3 points1[4];
		points1[0] = m_boundingBox.m_points[0];
		points1[3] = CVector3((m_boundingBox.m_points[0].X + m_boundingBox.m_points[3].X) / 2.0f, 0, (m_boundingBox.m_points[0].Z + m_boundingBox.m_points[3].Z) / 2.0f);

		points1[1] = CVector3(points1[3].X, 0, points1[0].Z);
		points1[2] = CVector3(points1[0].X, 0, points1[3].Z);
		aabb1.setPoints(points1);
		m_n1->setAABB(aabb1);


		AABB aabb2;
		CVector3 points2[4];
		points2[0] = CVector3((m_boundingBox.m_points[0].X + m_boundingBox.m_points[3].X) / 2.0f, 0, m_boundingBox.m_points[0].Z);
		points2[3] = CVector3(m_boundingBox.m_points[3].X, 0, (m_boundingBox.m_points[0].Z + m_boundingBox.m_points[3].Z) / 2.0f);

		points2[1] = CVector3(points2[3].X, 0, points2[0].Z);
		points2[2] = CVector3(points2[0].X, 0, points2[3].Z);
		aabb2.setPoints(points2);
		m_n2->setAABB(aabb2);


		AABB aabb3;
		CVector3 points3[4];
		points3[0] = CVector3(m_boundingBox.m_points[0].X, 0, (m_boundingBox.m_points[0].Z + m_boundingBox.m_points[3].Z) / 2.0f);
		points3[3] = CVector3((m_boundingBox.m_points[0].X + m_boundingBox.m_points[3].X) / 2.0f, 0, m_boundingBox.m_points[3].Z);

		points3[1] = CVector3(points3[3].X, 0, points3[0].Z);
		points3[2] = CVector3(points3[0].X, 0, points3[3].Z);
		aabb3.setPoints(points3);
		m_n3->setAABB(aabb3);


		AABB aabb4;
		CVector3 points4[4];
		points4[0] = CVector3((m_boundingBox.m_points[0].X + m_boundingBox.m_points[3].X) / 2.0f, 0, (m_boundingBox.m_points[0].Z + m_boundingBox.m_points[3].Z) / 2.0f);
		points4[3] = CVector3(m_boundingBox.m_points[3].X, 0, m_boundingBox.m_points[3].Z);

		points4[1] = CVector3(points3[3].X, 0, points3[0].Z);
		points4[2] = CVector3(points3[0].X, 0, points3[3].Z);
		aabb4.setPoints(points4);
		m_n4->setAABB(aabb4);


		if (points1[0].X == points1[1].X)
		{
			return;
		}

		m_data.clear();
		m_n1->subDivide(limit, grid);
		m_n2->subDivide(limit, grid);
		m_n3->subDivide(limit, grid);
		m_n4->subDivide(limit, grid);
	}
}
