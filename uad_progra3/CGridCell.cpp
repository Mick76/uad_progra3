#include "CGridCell.h"



CGridCell::CGridCell()
{
}

CGridCell::CGridCell(int index, CVector3 cellPosition, int x, int y, bool hasModel)
{
	m_index = index;
	m_cellPosition = cellPosition;
	xCoord = x;
	yCoord = y;
	m_hasModel = hasModel;
}

CGridCell::~CGridCell()
{
}
