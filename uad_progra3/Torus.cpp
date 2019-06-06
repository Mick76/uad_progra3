#include "stdafx.h"

#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES

#include <assert.h>
#include <math.h>
#include "Include/Globals.h"
#include "Torus.h"
#include "Include/CWideStringHelper.h"

/* */
Torus::Torus() :
	Torus(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
Torus::Torus(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_currentDeltaTime{ 0.0 },
	m_objectRotation{ 0.0 },
	m_objectPosition{ 0.0f, -1.0f, 0.0f },
	m_rotationSpeed{ DEFAULT_ROTATION_SPEED },
	m_torusVertexArrayObject{ 0 },
	m_numFacesTorus{ 0 },
	m_renderPolygonMode{ 0 }

{
	cout << "Constructor: Torus(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================

}

/* */
Torus::~Torus()
{
	cout << "Destructor: ~Torus()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================
	if (m_textureID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_textureID);
	}

	if (m_torusVertexArrayObject > 0)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_torusVertexArrayObject);
	}
}

/* */
void Torus::initialize()
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
	createTorusGeometry();
}

/* */
void Torus::run()
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
			getOpenGLRenderer()->setClearScreenColor(0.0f, 0.0f, 0.0f);

			// Initialize window width/height in the renderer
			getOpenGLRenderer()->setWindowWidth(getGameWindow()->getWidth());
			getOpenGLRenderer()->setWindowHeight(getGameWindow()->getHeight());

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
void Torus::update(double deltaTime)
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
void Torus::render()
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
		float color[3] = { 0.0f, 1.0f, 0.0f };
		unsigned int noTexture = 0;

		// convert total degrees rotated to radians;
		double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

		// Get a matrix that has both the object rotation and translation
		MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, m_objectPosition);

		if (m_torusVertexArrayObject > 0 && m_numFacesTorus > 0)
		{

			// Render pyramid in the first position, using the color shader
			getOpenGLRenderer()->renderObject(
				&m_colorModelShaderId,
				&m_torusVertexArrayObject,
				&noTexture,
				m_numFacesTorus,
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}

	}
}

void Torus::swap(float *xp, float *yp)
{
	float temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void Torus::createTorusGeometry()
{
	float radio = 2.0f;
	bool loaded = false;
	float v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3];

	const int aros = 32;
	const int verticesCircle = 32;//vertices por circulo
	const int verticesTotales = verticesCircle * aros;//vertices totales
	const int carasCircle = verticesCircle; // vertices * canitidad de aros
	const int carasTotales = verticesCircle * aros * 2; // vertices * canitidad de aros

	float vData[verticesTotales * 3];
	double slice = 2 * M_PI / verticesCircle;
	double slice2 = M_PI / aros*4;

	//anadir vertices de circulo
	for (int e = 0; e < aros; e++)
	{
		double angle2 = M_PI / 2 - e * slice2 ;

		for (int i = e * verticesCircle * 3; i < e*verticesCircle * 3 + (verticesCircle * 3); i++)
		{
			double angle = slice * i;

			float newX = (float)((radio + ((radio / 2) *cosf(angle))) *cosf(angle2));
			float newY = (float)((radio + ((radio / 2) *cosf(angle))) *sinf(angle2));
			float newZ = (float)((radio / 2) *sinf(angle));


			vData[i] = newX;
			vData[i + 1] = newY;
			vData[i + 2] = newZ;
			i += 2;
		}
	}



	float vertexUVs[verticesTotales * 2] =
	{
		0.5f,  0.11f, // TOP 
		0.5f,  0.11f, // BOTTOM LEFT, FRONT
		0.5f,  0.11f, // BOTTOM RIGHT, FRONT
		0.5f,  0.11f, // BOTTOM LEFT, BACK
		0.5f,  0.11f,  // BOTTOM RIGHT, BACK 
		0.5f,  0.11f  // Down 
	};

	m_numFacesTorus = carasTotales;
	unsigned short tIndices[carasTotales * 3];///////////////////////////////

	//hacer las caras
	int max = 0;
	int val = 0;
	int val2 = 0;
	for (int e = 0; e < aros; e++)
	{
		val2 = 0;
		for (max = e * carasCircle * 3; max <= e * verticesCircle * 3 + (carasCircle * 3) - 1; max++)
		{
			tIndices[max] = val;
			if (e < aros - 1)
			{
				tIndices[max + 1] = val2 + ((e + 1) * carasCircle);
			}
			else if (e == aros - 1)
			{
				tIndices[max + 1] = val2 + ((e)* carasCircle);
			}
			tIndices[max + 2] = val + 1;
			max += 2;
			val++;
			val2++;
		}
		tIndices[e*verticesCircle * 3 + (carasCircle * 3) - 1] = tIndices[e * carasCircle * 3];
	}

	//hacer mas caras

	val = 0;
	val2 = 0;
	for (int e = 0; e < aros; e++)
	{
		val2 = 0;
		for (int i = max + (e * carasCircle * 3); i <= max + e * verticesCircle * 3 + (carasCircle * 3) - 3; i++)
		{
			if (e < aros - 1)
			{
				tIndices[i] = val + 1;
				tIndices[i + 1] = val2 + 1 + ((e + 1) * carasCircle);
				tIndices[i + 2] = val2 + ((e + 1) * carasCircle);
			}
			else
			{
				tIndices[i] = val;
				tIndices[i + 1] = val2 + ((e)* carasCircle);
				tIndices[i + 2] = val2 + ((e)* carasCircle);
			}
			i += 2;
			val++;
			val2++;
		}
		tIndices[max + (e * verticesCircle * 3 + (carasCircle * 3)) - 2] = tIndices[max + (e * carasCircle * 3) + 2];
	}



	float nData[carasTotales * 3];

	for (int n = 0; n < carasTotales * 3; n++)
	{
		nData[n] = 0.0f;
	}

	unsigned short nIndices[carasTotales * 3];

	int nVal = 0;
	for (int n = 0; n < carasTotales * 3; n += 3)
	{
		nIndices[n] = nVal;
		nIndices[n + 1] = nVal;
		nIndices[n + 2] = nVal;
		nVal++;
	}

	for (int i = 0; i < m_numFacesTorus; i++)
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
		&m_torusVertexArrayObject,
		vData,
		verticesTotales,        // Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		nData,
		carasTotales,
		vertexUVs,
		verticesTotales,
		tIndices,
		carasTotales,        // Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		nIndices,
		carasTotales,
		tIndices,
		carasTotales
	);

	if (!loaded)
	{
		m_numFacesTorus = 0;

		if (m_torusVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_torusVertexArrayObject);
			m_torusVertexArrayObject = 0;
		}
	}
}



void Torus::normcrossprod(float v1[3], float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];

	normalize(out);
}

/* */
void Torus::normalize(float v[3])
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
void Torus::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void Torus::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

void Torus::onF3(int mods)
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