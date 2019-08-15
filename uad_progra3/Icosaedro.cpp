#include "stdafx.h"

#include <iostream>
using namespace std;

#include <assert.h>
#include "Include/Globals.h"
#include "Icosaedro.h"
#include "Include/CWideStringHelper.h"

/* */
Icosaedro::Icosaedro() :
	Icosaedro(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
Icosaedro::Icosaedro(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_currentDeltaTime{ 0.0 },
	m_objectRotation{ 0.0 },
	m_objectPosition{ 0.0f, -1.0f, 0.0f },
	m_rotationSpeed{ DEFAULT_ROTATION_SPEED },
	m_icosaedroVertexArrayObject{ 0 },
	m_numFacesIcosaedro{ 0 },
	m_renderPolygonMode{ 0 }

{
	cout << "Constructor: Icosaedro(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================

}

/* */
Icosaedro::~Icosaedro()
{
	cout << "Destructor: ~Icosaedro()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================
	if (m_textureID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_textureID);
	}

	if (m_icosaedroVertexArrayObject > 0)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_icosaedroVertexArrayObject);
	}
}

/* */
void Icosaedro::initialize()
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

	m_initialized = true;
	createIcosaedroGeometry();
}

/* */
void Icosaedro::run()
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
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Initialize window width/height in the renderer
			//getOpenGLRenderer()->setWindowWidth(getGameWindow()->getWidth());
			//getOpenGLRenderer()->setWindowHeight(getGameWindow()->getHeight());

			if (m_initialized)
			{
				getOpenGLRenderer()->setWireframePolygonMode();

				// Enter main loop
				cout << "Entering Main loop" << endl;
				getGameWindow()->mainLoop(this);
			}
		}
	}
}

/* */
void Icosaedro::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//
	double degreesToRotate = 0.0;

	// Save current delta time
	m_currentDeltaTime = deltaTime;

	// Calculate degrees to rotate
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// degrees = rotation speed * delta time 
	// deltaTime is expressed in milliseconds, but our rotation speed is expressed in seconds (convert delta time from milliseconds to seconds)
	degreesToRotate = m_rotationSpeed * (deltaTime / 1000.0);
	// accumulate rotation degrees
	m_objectRotation += degreesToRotate;

	// Reset rotation if needed
	while (m_objectRotation > 360.0)
	{
		m_objectRotation -= 360.0;
	}
	if (m_objectRotation < 0.0)
	{
		m_objectRotation = 0.0;
	}
	// ===============================
}

/* */
void Icosaedro::render()
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

		// Get a matrix that has both the object rotation and translation
		//MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, m_objectPosition);
		/*
		if (m_icosaedroVertexArrayObject > 0 && m_numFacesIcosaedro > 0)
		{

			// Render pyramid in the first position, using the color shader
			getOpenGLRenderer()->renderObject(
				&m_colorModelShaderId,
				&m_icosaedroVertexArrayObject,
				&noTexture,
				m_numFacesIcosaedro,
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}
		*/

	}
}

void Icosaedro::createIcosaedroGeometry()
{
	float height = 1.0f;
	float LongitudArista = 1.0f;
	bool loaded = false;
	float v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3];

	float vData[36] = {
		0.0, -height , 0.0,                // Down //0

		0.0, 0.0, LongitudArista + LongitudArista / 2,//1
		LongitudArista * 2, 0.0, LongitudArista / 2,//2
		LongitudArista, 0.0, -LongitudArista - LongitudArista / 2,//3
		-LongitudArista,0.0, -LongitudArista - LongitudArista / 2,//4
		-LongitudArista * 2,0.0, LongitudArista / 2,//5

		LongitudArista, height * 2, LongitudArista + LongitudArista / 2,
		LongitudArista * 2, height * 2, -LongitudArista / 2,
		0.0, height * 2,-LongitudArista - LongitudArista / 2,
		-LongitudArista * 2, height * 2, -LongitudArista / 2,
		-LongitudArista, height * 2, LongitudArista + LongitudArista / 2,

		0.0, height * 3, 0.0                // TOP /11
	};

	float vertexUVs[24] =
	{
		0.5f,  0.11f, // TOP 
		0.5f,  0.11f, // BOTTOM LEFT, FRONT
		0.5f,  0.11f, // BOTTOM RIGHT, FRONT
		0.5f,  0.11f, // BOTTOM LEFT, BACK
		0.5f,  0.11f,  // BOTTOM RIGHT, BACK 
		0.5f,  0.11f,  // Down 
		0.5f,  0.11f,  // Down 
		0.5f,  0.11f,  // Down 
		0.5f,  0.11f,  // Down 
		0.5f,  0.11f,  // Down 
		0.5f,  0.11f,  // Down 
		0.5f,  0.11f  // Down 
	};

	m_numFacesIcosaedro = 20;

	unsigned short tIndices[60] = {
		0, 1, 2,
		0, 2, 3,
		0,3,4,
		0,4,5,
		0,5,1,

		2,6,1,
		3,7,2,
		4,8,3,
		5,9,4,
		1,10,5,

		7,6,11,
		8,7,11,
		9,8,11,
		10,9,11,
		6,10,11,

		7,2,6,
		8,3,7,
		9,4,8,
		10,5,9,
		6,1,10
	};

	float nData[60] = {
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0
	};

	unsigned short nIndices[60] = {
		0, 0, 0,
		1, 1, 1,
		2, 2, 2,
		3, 3, 3,
		4, 4, 4,
		5, 5, 5,
		6, 6, 6,
		7,7,7,
		8,8,8,
		9,9,9,
		10,10,10,
		11,11,11,
		12,12,12,
		13,13,13,
		14,14,14,
		15,15,15,
		16,16,16,
		17,17,17,
		18,18,18,
		19,19,19
	};

	for (int i = 0; i < m_numFacesIcosaedro; i++)
	{
		// Vertex 1
		v1[0] = vData[tIndices[i * 3]];
		v1[1] = vData[tIndices[i * 3] + 1];
		v1[2] = vData[tIndices[i * 3] + 2];

		// Vertex 2
		v2[0] = vData[tIndices[(i * 3) + 1]];
		v2[1] = vData[tIndices[(i * 3) + 1] + 1];
		v2[2] = vData[tIndices[(i * 3) + 1] + 2];

		// Vertex 3
		v3[0] = vData[tIndices[(i * 3) + 2]];
		v3[1] = vData[tIndices[(i * 3) + 2] + 1];
		v3[2] = vData[tIndices[(i * 3) + 2] + 2];

		// Vector from v2 to v1
		v1v2[0] = v1[0] - v2[0];
		v1v2[1] = v1[1] - v2[1];
		v1v2[2] = v1[2] - v2[2];

		// Vector from v2 to v3
		v1v3[0] = v3[0] - v2[0];
		v1v3[1] = v3[1] - v2[1];
		v1v3[2] = v3[2] - v2[2];

		normcrossprod(v1v2, v1v3, norm);

		nData[i * 3] = norm[0];
		nData[(i * 3) + 1] = norm[1];
		nData[(i * 3) + 2] = norm[2];
	}

	// Allocate graphics memory for object
	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_icosaedroVertexArrayObject,
		vData,
		12,        // Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		nData,
		20,
		vertexUVs,
		12,
		tIndices,
		20,        // Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		nIndices,
		20,
		tIndices,
		20
	);

	if (!loaded)
	{
		m_numFacesIcosaedro = 0;

		if (m_icosaedroVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_icosaedroVertexArrayObject);
			m_icosaedroVertexArrayObject = 0;
		}
	}
}

void Icosaedro::normcrossprod(float v1[3], float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];

	normalize(out);
}

/* */
void Icosaedro::normalize(float v[3])
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
void Icosaedro::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void Icosaedro::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

void Icosaedro::onF3(int mods)
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