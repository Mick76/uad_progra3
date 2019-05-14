#pragma once

#ifndef CAPPIcosaedro_H
#define CAPPIcosaedro_H

#include "Include/Globals.h"
#include "Include/CApp.h"

// --------------------------------------------------------------------------------------------------------------------------------------
// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
// --------------------------------------------------------------------------------------------------------------------------------------
class Icosaedro : public CApp
{
private:

	// ---------------------------------------------------
	// Private member variables specific to CAppEmpty only
	// ---------------------------------------------------
	//
	// ---------------------------------------------------
	bool m_initialized;

	// Variables used for controlling the rotation and object position
	// ---------------------------------------------------------------

	// Current delta time (time of the current frame - time of the last frame)
	double m_currentDeltaTime;

	// Current object rotation, expressed in degrees
	double m_objectRotation;

	// Current object position
	CVector3 m_objectPosition;

	//  Object rotation speed (degrees per second)
	double m_rotationSpeed;

	int m_numFacesIcosaedro;
	int m_renderPolygonMode;

	// OpenGL Shader Program for the color-only object
	unsigned int m_colorModelShaderId;

	// OpenGL Shader Program for the textured object
	unsigned int m_texturedModelShaderId;

	// An OpenGL Vertex Array Object is a reference to graphics memory that tells OpenGL where to look for the geometry of the object
	// It needs to be initialized using the allocateGraphicsMemory() method from the renderer.
	unsigned int m_icosaedroVertexArrayObject;

	// Identifier for the OpenGL Texture Object 
	unsigned int m_textureID;

protected:

	// Method to initialize the menu 
	// (not needed in CAppEmpty, so return false)
	bool initializeMenu() { return false; }

public:
	// Constructors and destructor
	Icosaedro();
	Icosaedro(int window_width, int window_height);
	~Icosaedro();

	// --------------------------------------------------------------------------------------------------------------------------
	// Inherited methods from CApp
	// Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
	// --------------------------------------------------------------------------------------------------------------------------

	// Method to initialize any objects for this class
	void initialize();

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// ------------------------------------------------------------------------------------------------------------------------
	// Other inherited methods from CApp. These are optional to override.
	// Only provide an implementation in this DERIVED class if you need them to do something different than the base class CApp
	// -----------------------------------------------------------------------------------------------------------------------

	// Executes the action corresponding to the selected menu item
	void executeMenuAction();

	// Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);

	void onF3(int mods);

	// -----------------------------------------
	// Public methods specific to CAppEmpty only
	// -----------------------------------------
	//
	// -----------------------------------------

private:

	// ------------------------------------------
	// Private methods specific to CAppEmpty only
	// ------------------------------------------
	//
	// ------------------------------------------
	void Icosaedro::createIcosaedroGeometry();
	void normcrossprod(float v1[3], float v2[3], float out[3]);
	void normalize(float v[3]);
};

#endif // !CAPPIcosaedro_H

