#pragma once
#include "Include/CApp.h"

class CGridCell
{
public:

	CVector3 m_cellPosition;

	int xCoord;
	int yCoord;

	int m_index;
	bool m_hasModel = false;

	CGridCell();
	CGridCell(int index, CVector3 cellPosition, int x, int y, bool hasModel);
	~CGridCell();
};

