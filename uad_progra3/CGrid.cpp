#include "CGrid.h"



CGrid::CGrid(int columns, int rows, float sizecell)
{
	m_columns = columns;
	m_rows = rows;
	m_sizeHex = sizecell;
}


CGrid::~CGrid()
{
}

void CGrid::initialize()
{
	float w = sqrt(3) * m_sizeHex;
	float h = 2 * m_sizeHex * 0.75f;

	for (int row = 0; row < m_rows; row++)
	{
		for (int column = 0; column < m_columns; column++)
		{
			if (row % 2 == 0)
			{
				m_cells.push_back(CGridCell(-1, CVector3(w * column, 0, h * row), column, row, false));
			}
			else
			{
				m_cells.push_back(CGridCell(-1, CVector3(w*column + (w * 0.5), 0, h * row), column, row, false));
			}
		}
	}

	int x = 0;
	for (int i = 0; i < 18; i += 3)
	{
		vData[i] = m_sizeHex * cosf(M_PI / 180 * (60 * x - 30));
		vData[i + 1] = 0;
		vData[i + 2] = m_sizeHex * sinf(M_PI / 180 * (60 * x - 30));
		x++;
	}

	//getFromCoords(1, 1)->m_cellPosition.setValues(getFromCoords(1, 1)->m_cellPosition.X, 0.2f, getFromCoords(1, 1)->m_cellPosition.Z);
}

CGridCell * CGrid::getFromCoords(int x, int y)
{
	return &m_cells[x + (y*m_rows)];
}
