#include "stdafx.h"

#include "windows.h"

#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES

#include <assert.h>
#include "Include/Globals.h"
#include "CAppFullGrid.h"
#include "../CAppObjLoader.h"
#include "Include/CWideStringHelper.h"
#include <math.h>
#include "CGrid.h"

/* */
CAppFullGrid::CAppFullGrid() :
	CAppFullGrid(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CAppFullGrid::CAppFullGrid(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_currentDeltaTime{ 0.0 },
	m_objectRotation{ 0.0 },
	m_objectPosition{ 0.0f, -1.0f, 0.0f },
	m_rotationSpeed{ DEFAULT_ROTATION_SPEED },
	m_VertexArrayObject{ 0 },
	m_numFaces{ 0 },
	m_renderPolygonMode{ 0 }

{
	cout << "Constructor: CAppHex(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================

}

/* */
CAppFullGrid::~CAppFullGrid()
{
	cout << "Destructor: ~CAppHex()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================
	if (m_textureID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_textureID);
	}

	if (m_VertexArrayObject > 0)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_VertexArrayObject);
	}

	if (m_grid != nullptr)
	{
		delete m_grid;

	}
}

/* */
void CAppFullGrid::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	//
	std::wstring wresourceFilenameVS, wresourceFilenameFS, wresourceFilenameTexture;
	std::string resourceFilenameVS, resourceFilenameFS, resourceFilenameTexture;

	// Color Shader
	// Load shader file, create OpenGL Shader Object for it
	// -------------------------------------------------------------------------------------------------------------

	// Check shader for the color-only object exists
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_3D_OBJECT, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_3D_OBJECT, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_3D_OBJECT << endl;
		cout << "  " << FRAGMENT_SHADER_3D_OBJECT << endl;

		return;
	}

	if (!getOpenGLRenderer()->createShaderProgram(
		&m_colorModelShaderId,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()))
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return;
	}

	// Texture Shader
	// Load shader file, create OpenGL Shader Object for it
	// -------------------------------------------------------------------------------------------------------------

	// Check shader for the textured object exists
	wresourceFilenameFS.clear();
	wresourceFilenameVS.clear();
	resourceFilenameFS.clear();
	resourceFilenameVS.clear();
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_TEXTURED_3D_OBJECT, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_TEXTURED_3D_OBJECT, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_TEXTURED_3D_OBJECT << endl;
		cout << "  " << FRAGMENT_SHADER_TEXTURED_3D_OBJECT << endl;

		return;
	}

	if (!getOpenGLRenderer()->createShaderProgram(
		&m_texturedModelShaderId,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()))
	{
		cout << "ERROR: Unable to load texture shader" << endl;
		return;
	}

	// Texture
	// Load texture file, create OpenGL Texture Object
	// -------------------------------------------------------------------------------------------------------------

	// Check texture file exists for the textured cube
	if (!CWideStringHelper::GetResourceFullPath(MC_WOOD_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_WOOD_TEXTURE << endl;
		return;
	}

	// Initialize the texture
	m_textureID = 0;
	if (!loadTexture(resourceFilenameTexture.c_str(), &m_textureID))
	{
		cout << "ERROR: Unable load texture:" << endl;
		cout << "  " << MC_WOOD_TEXTURE << endl;
		return;
	}

	//`"C:\\Users\\bluem\\Documents\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\MODELS\\TF2\\TF2_REF_SOLDIER_yz.obj"
	m_objLoader = new CAppObjLoader(800, 600);

	m_initialized = true;
	m_grid = new CGrid(0, 0, 0);
	onF2(0);//load map on start
	m_grid->initialize();
	createHexGeometry();
	quadtree = new Quadtree<CGridCell>(6, m_grid->m_cells);

	m_camera = new CCamera();
	m_camera->m_pos.Y = 10.0f;
	m_camera->m_pos.Z = 8.0f;
	m_camera->m_upVec.Y = 1.0f;
	m_camera->m_rightVec.X = 1;
}

/* */
void CAppFullGrid::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(1.0f, 1.0f, 1.0f);

			// Initialize window width/height in the renderer
			//getOpenGLRenderer()->setWindowWidth(getGameWindow()->getWidth());
			//getOpenGLRenderer()->setWindowHeight(getGameWindow()->getHeight());

			if (m_initialized)
			{
				getOpenGLRenderer()->setFillPolygonMode();
				m_renderPolygonMode = 1;

				// Enter main loop
				cout << "Entering Main loop" << endl;
				getGameWindow()->mainLoop(this);
			}
		}
	}
}

/* */
void CAppFullGrid::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//

	// Save current delta time
	m_currentDeltaTime = deltaTime;
}

/* */
void CAppFullGrid::render()
{
	CGameMenu *menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
		//...
	}
	else // Otherwise, render app-specific stuff here...
	{
		// =================================
		//
		//white
		// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
		float color[3] = { 1.0f, 1.0f, 1.0f };
		unsigned int noTexture = 0;
		// convert total degrees rotated to radians;
		double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

		// Construct a view matrix
		m_camera->m_viewMatrix = MathHelper::ViewMatrix(
			m_camera->m_pos, m_camera->m_lookAt, m_camera->m_upVec
		);

		int tmpW = getOpenGLRenderer()->getFramebufferWidth();
		int tmpH = getOpenGLRenderer()->getFramebufferHeight();
		float aspectRatio = (float)getOpenGLRenderer()->getFramebufferWidth() / (float)getOpenGLRenderer()->getFramebufferHeight();

		// Construct a projection matrix
		m_camera->m_projMatrix = MathHelper::PerspectiveProjectionMatrix(
			75.0f, aspectRatio, 0.1f, 100.0f
		);


		m_camera->m_frustum.update(m_camera->m_pos, m_camera->m_lookAt, m_camera->m_upVec, m_camera->m_rightVec, 1, 400, 60, aspectRatio);

		// Get a matrix that has both the object rotation and translation

		vector <CGridCell*> visibleObjects;

		quadtree->render(m_camera, &visibleObjects);

		if (m_VertexArrayObject > 0 && m_numFaces > 0)
		{
			for (int i = 0; i < visibleObjects.size(); i++)
			{
				m_objectPosition = visibleObjects[i]->m_cellPosition;
				MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, m_objectPosition);

				if (visibleObjects[i] == m_grid->getFromCoords(currentX, currentY))
				{
					color[0] = 1;
					color[1] = 0;
					color[2] = 0;
				}
				else
				{
					color[0] = 1;
					color[1] = 1;
					color[2] = 1;
				}

				getOpenGLRenderer()->renderObjectCamera(
					&m_camera->m_viewMatrix,
					&m_camera->m_projMatrix,
					&m_colorModelShaderId,
					&m_VertexArrayObject,
					&noTexture,
					m_numFaces,
					color,
					&modelMatrix,
					COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
					false
				);


				if (visibleObjects[i]->m_hasModel)
				{
					float scale = m_objInstances[visibleObjects[i]->m_index].m_scale;

					MathHelper::Matrix4 scaleMatrix(
						scale, 0.0f, 0.0f, 0.0f,
						0.0f, scale, 0.0f, 0.0f,
						0.0f, 0.0f, scale, 0.0f,
						m_objectPosition.getX(), m_objectPosition.getY(), m_objectPosition.getZ(), 1.0f);

					getOpenGLRenderer()->renderObjectCamera(
						&m_camera->m_viewMatrix,
						&m_camera->m_projMatrix,
						&m_colorModelShaderId,
						m_gameobjects[m_objInstances[visibleObjects[i]->m_index].m_objectIndex]->getGraphicsMemoryObjectId(),
						&noTexture,
						m_gameobjects[m_objInstances[visibleObjects[i]->m_index].m_objectIndex]->getNumFaces(),
						color,
						&scaleMatrix,
						COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
						false
					);
				}
			}
		}

		/*
		if (m_VertexArrayObject > 0 && m_numFaces > 0)
		{
			for (int i = 0; i < m_grid->m_cells.size(); i++)
			{
				m_objectPosition = m_grid->m_cells[i]->m_cellPosition;
				MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, m_objectPosition);

				if (i == currentX + ((currentY)*m_grid->m_rows))
				{
					color[0] = 1;
					color[1] = 0;
					color[2] = 0;
				}
				else
				{
					color[0] = 1;
					color[1] = 1;
					color[2] = 1;
				}
				
				getOpenGLRenderer()->renderObjectCamera(
					&m_camera->m_viewMatrix,
					&m_camera->m_projMatrix,
					&m_colorModelShaderId,
					&m_VertexArrayObject,
					&noTexture,
					m_numFaces,
					color,
					&modelMatrix,
					COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
					false
				);
				

				if (m_grid->m_cells[i]->m_hasModel)
				{
					float scale = m_objInstances[m_grid->m_cells[i]->m_index].m_scale;

					MathHelper::Matrix4 scaleMatrix(
						scale, 0.0f, 0.0f, 0.0f,
						0.0f, scale, 0.0f, 0.0f,
						0.0f, 0.0f, scale, 0.0f,
						m_objectPosition.getX(), m_objectPosition.getY(), m_objectPosition.getZ(), 1.0f);

					getOpenGLRenderer()->renderObjectCamera(
						&m_camera->m_viewMatrix,
						&m_camera->m_projMatrix,
						&m_colorModelShaderId,
						m_gameobjects[m_objInstances[m_grid->m_cells[i]->m_index].m_objectIndex]->getGraphicsMemoryObjectId(),
						&noTexture,
						m_gameobjects[m_objInstances[m_grid->m_cells[i]->m_index].m_objectIndex]->getNumFaces(),
						color,
						&scaleMatrix,
						COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
						false
					);
				}
			}
		}
		*/

	}
}

void CAppFullGrid::createHexGeometry()
{
	bool loaded = false;

	m_numFaces = 4;

	// Allocate graphics memory for object

	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_VertexArrayObject,
		m_grid->vData,
		6,        // Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		m_grid->nData,
		4,
		m_grid->vertexUVs,
		6,
		m_grid->tIndices,
		4,        // Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		m_grid->nIndices,
		4,
		m_grid->tIndices,
		4
	);

	if (!loaded)
	{
		m_numFaces = 0;

		if (m_VertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_VertexArrayObject);
			m_VertexArrayObject = 0;
		}
	}
}

void CAppFullGrid::normcrossprod(float v1[3], float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];

	normalize(out);
}

/* */
void CAppFullGrid::normalize(float v[3])
{
	float d = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (d == 0.0f)
	{
		return;
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}


/* */
void CAppFullGrid::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

void CAppFullGrid::onF3(int mods)
{
	if (m_renderPolygonMode == 0)
	{
		getOpenGLRenderer()->setFillPolygonMode();
		m_renderPolygonMode = 1;
	}
	else
	{
		getOpenGLRenderer()->setWireframePolygonMode();
		m_renderPolygonMode = 0;
	}
}

void CAppFullGrid::onF2(int mods)
{
	if (mods & KEY_MOD_SHIFT)
	{
		m_objLoader->openObjFile(false);
		m_gameobjects.push_back(m_objLoader->getObject());
	}
	else
	{
		std::wstring wideStringBuffer = L"";
		wideStringBuffer.resize(MAX_PATH);

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = L" Json Files\0*.json\0 All files\0*.*\0";
		ofn.lpstrFile = &wideStringBuffer[0];
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = L"Select a json file";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), NULL, 0, NULL, NULL);
			std::string multibyteString(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), &multibyteString[0], size_needed, NULL, NULL);
			cout << "Filename to load: " << multibyteString.c_str() << endl;
			loadMap(multibyteString.c_str());
		}
	}
}

void CAppFullGrid::onF4(int mods)
{
	// Check BITWISE AND to detect shift/alt/ctrl
	if (mods & KEY_MOD_SHIFT)
	{
		moveCamera(-3);
	}
	else
	{
		moveCamera(3);
	}
}

void CAppFullGrid::onF5(int mods)
{
	if (m_gameobjects.size() > 0)
	{
		if (m_grid->getFromCoords(currentX, currentY)->m_hasModel == true)
		{
			m_grid->getFromCoords(currentX, currentY)->m_hasModel = false;
		}
		else
		{
			m_grid->getFromCoords(currentX, currentY)->m_hasModel = true;

			if (m_grid->getFromCoords(currentX, currentY)->m_index == -1)//si aun no se ha crado una instancia para el
			{
				m_objInstances.push_back(CObjectInstance(0, 0.05f));
				m_grid->getFromCoords(currentX, currentY)->m_index = m_objInstances.size() - 1;
			}
		}
	}
}

void CAppFullGrid::onF6(int mods)
{
	if (m_grid->getFromCoords(currentX, currentY)->m_hasModel == true)
	{
		if (m_objInstances[m_grid->getFromCoords(currentX, currentY)->m_index].m_objectIndex == m_gameobjects.size() - 1)
		{
			m_objInstances[m_grid->getFromCoords(currentX, currentY)->m_index].m_objectIndex = 0;
		}
		else
		{
			m_objInstances[m_grid->getFromCoords(currentX, currentY)->m_index].m_objectIndex++;
		}
	}
}

void CAppFullGrid::onF7(int mods)
{
	if (m_grid->getFromCoords(currentX, currentY)->m_hasModel == true)
	{
		if (mods & KEY_MOD_SHIFT)
		{
			m_objInstances[m_grid->getFromCoords(currentX, currentY)->m_index].m_scale -= m_objInstances[m_grid->getFromCoords(currentX, currentY)->m_index].m_scale*0.1f;
		}
		else
		{
			m_objInstances[m_grid->getFromCoords(currentX, currentY)->m_index].m_scale += m_objInstances[m_grid->getFromCoords(currentX, currentY)->m_index].m_scale*0.1f;
		}
	}
}


void CAppFullGrid::onArrowUp(int mods)
{
	if (mods & KEY_MOD_SHIFT)
	{
		if (currentX + ((currentY - 1)*m_grid->m_rows) >= 0)
		{
			currentY--;
		}
		Sleep(50);
	}
	else
	{
		m_camera->m_pos.Z -= 1;
		m_camera->m_lookAt.Z -= 1;
	}
}

void CAppFullGrid::onArrowDown(int mods)
{
	if (mods & KEY_MOD_SHIFT)
	{
		if (currentX + ((currentY + 1)*m_grid->m_rows) <= m_grid->m_cells.size() - 1)
		{
			currentY++;
		}
		Sleep(50);
	}
	else
	{
		m_camera->m_pos.Z += 1;
		m_camera->m_lookAt.Z += 1;
	}
}

void CAppFullGrid::onArrowLeft(int mods)
{
	if (mods & KEY_MOD_SHIFT)
	{
		if (currentX - 1 >= 0)
		{
			currentX--;
		}
		Sleep(50);
	}
	else
	{
		m_camera->m_pos.X -= 1;
		m_camera->m_lookAt.X -= 1;
	}
}
void CAppFullGrid::onArrowRight(int mods)
{
	if (mods & KEY_MOD_SHIFT)
	{
		if (currentX + 1 <= m_grid->m_rows - 1)
		{
			currentX++;
		}
		Sleep(50);
	}
	else
	{
		m_camera->m_pos.X += 1;
		m_camera->m_lookAt.X += 1;
	}
}

void CAppFullGrid::onMouseMove(float deltaX, float deltaY)
{
	/*
	if (deltaX < 100.0f && deltaY < 100.0f)
	{
		float moveX = -deltaX * DEFAULT_CAMERA_MOVE_SPEED;
		float moveZ = -deltaY * DEFAULT_CAMERA_MOVE_SPEED;

		float currPos[3];
		for (int i = 0; i < m_grid->m_cells.size(); i++)
		{
			m_grid->m_cells[i]->m_cellPosition.getValues(currPos);
			currPos[0] += moveX;
			currPos[2] += moveZ;
			m_grid->m_cells[i]->m_cellPosition.setValues(currPos);
		}
	}
	*/
}

void CAppFullGrid::moveCamera(float direction)
{
	if (getOpenGLRenderer() != NULL)
	{
		getOpenGLRenderer()->moveCamera(direction);
	}
}

void CAppFullGrid::loadMap(string path)
{
	//read a JSON file
	std::ifstream i(path);
	json j;
	i >> j;

	m_grid->m_columns = j["HexGrid"]["numCols"];
	m_grid->m_rows = j["HexGrid"]["numRows"];
	m_grid->m_sizeHex = j["HexGrid"]["cellSize"];
	m_grid->m_orientation = j["HexGrid"]["orientation"].get<string>();
}
