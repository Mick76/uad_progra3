#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>
#include "CGridCell.h"
using namespace std;

class CGrid
{
public:
	CGrid(int columns, int rows, float sizecell);
	~CGrid();
	void initialize();
	CGridCell * getFromCoords(int x, int y);

	int m_rows;
	int m_columns;
	float m_sizeHex = 0.5;
	string m_orientation;

	const int m_numFaces = 4;

	vector<CGridCell> m_cells;

	float vData[18] = {
	   m_sizeHex * cosf(M_PI / 180 * (60 * 0 - 30)), 0, m_sizeHex * sinf(M_PI / 180 * (60 * 0 - 30)),
	   m_sizeHex * cosf(M_PI / 180 * (60 * 1 - 30)), 0, m_sizeHex * sinf(M_PI / 180 * (60 * 1 - 30)),
	   m_sizeHex * cosf(M_PI / 180 * (60 * 2 - 30)), 0, m_sizeHex * sinf(M_PI / 180 * (60 * 2 - 30)),
	   m_sizeHex * cosf(M_PI / 180 * (60 * 3 - 30)), 0, m_sizeHex * sinf(M_PI / 180 * (60 * 3 - 30)),
	   m_sizeHex * cosf(M_PI / 180 * (60 * 4 - 30)), 0, m_sizeHex * sinf(M_PI / 180 * (60 * 4 - 30)),
	   m_sizeHex * cosf(M_PI / 180 * (60 * 5 - 30)), 0, m_sizeHex * sinf(M_PI / 180 * (60 * 5 - 30)),
	};

	float vertexUVs[12] =
	{
		0.5f,  0.11f,
		0.5f,  0.11f,
		0.5f,  0.11f,
		0.5f,  0.11f,
		0.5f,  0.11f,
		0.5f,  0.11f
	};

	unsigned short tIndices[12] = {
	   0,1,5,
	   1,2,4,
	   2,3,4,
	   4,1,5,
	};

	float nData[12] = {
	   0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0
	};

	unsigned short nIndices[12] = {
		0, 0, 0,
		1, 1, 1,
		2, 2, 2,
		3, 3, 3
	};

private:

};

