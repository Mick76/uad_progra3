// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
using namespace std;

#include "Include/CGameWindow.h"
#include "Include/CApp.h"
#include "Include/CAppCubeTest.h"
#include "Include/CAppObjLoader.h"
#include "Include/CAppGeometricFigures.h"
#include "Include/CAppEmpty.h"
#include "Esfera.h"
#include "Torus.h"
#include "Icosaedro.h"

int main()
{
	CApp *app = NULL;                  // Pointer to BASE class CApp
	//Aqui cambie por Torus, Icosaedro o Esfera
	app = new CAppObjLoader(800, 600);  // Using pointer to base class, create a new object of DERIVED class
	app->run();                        // Run the app
	delete app;                        // Delete pointer
	app = NULL;                        // Set pointer to NULL

	return 0;
}

