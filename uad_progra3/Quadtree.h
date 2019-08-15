#pragma once
#include <vector>
#include "QuadtreeNode.h"
#include "CCamera.h"
#include "AABB.h"

using namespace std;

template <class T>
class Quadtree
{
private:
	QuadtreeNode<T> *m_root;
	int m_triLimit;

public:
	Quadtree();
	Quadtree(int limit, vector<T*> grid);
	~Quadtree();

	void render(CCamera * cam, vector<T*> * visibleObjects);
};


template <class T>
Quadtree<T>::Quadtree()
{

}

template<class T>
Quadtree<T>::Quadtree(int limit, vector<T*> grid)
{

	AABB rootBox;
	m_root = new QuadtreeNode<T>();

	//calcular puntos de AABB
	CVector3 points[4];
	points[0] = grid[0]->m_cellPosition;
	points[3] = grid[grid.size()-1]->m_cellPosition;
	points[1] = CVector3(points[3].X, 0, points[0].Z);
	points[2] = CVector3(points[0].X, 0, points[3].Z);

	rootBox.setPoints(points);
	m_root->setAABB(rootBox);
	m_root->subDivide(limit, grid);
}

template <class T>
Quadtree<T>::~Quadtree()
{

}

template<class T>
inline void Quadtree<T>::render(CCamera * cam, vector<T*>* visibleObjects)
{
	m_root->render(cam, visibleObjects);
}
