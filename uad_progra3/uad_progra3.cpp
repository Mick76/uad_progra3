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
#include "CAppFullGrid.h"

//game engine architecture 3ra version de jason gregory capitulo 4

//poner esto en el archivo que lea el json. ej. HexWorld
#include "nlohmann/json.hpp"
#include <fstream>
#include <iomanip>
using json = nlohmann::json;

//como crear un archivo json
void jsonTest()
{
	//parse explicitly
	auto j = json::parse("{ \"happy\" : true, \"pi\":3.141 }");

	//explicit conversion to string
	std::string s = j.dump(); //{ \"happy\"} : true, \"pi\":3.141 }

	//serialization with pretty printing
	//pass in the amount of spaces to indent
	std::cout << j.dump(4) << std::endl;

	//read a JSON file
	std::ifstream i("C:\\Users\\bluem\\Documents\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\HEXGRID\\hexgrid_cfg.json");
	json j2;
	i >> j2;

	//serialization with pretty printing
	//pass in the amount of spaces to ident
	cout << "JSON configuration for CAppHexGrid" << endl;
	std::cout << j2.dump(4) << std::endl;
}

void jsonTest2()
{
	//read a JSON file
	std::ifstream i("C:\\Users\\bluem\\Documents\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\HEXGRID\\hexgrid_cfg.json");
	json j2;
	i >> j2;

	int aber = j2["HexGrid"]["numCols"];

	cout << " --------->" << aber << endl;
}


int main()
{
	//jsonTest2();

	
	CApp *app = NULL;                  // Pointer to BASE class CApp
	//Aqui cambie por Torus, Icosaedro o Esfera
	app = new CAppFullGrid(800, 600);  // Using pointer to base class, create a new object of DERIVED class
	app->run();                        // Run the app
	delete app;                        // Delete pointer
	app = NULL;                        // Set pointer to NULL
	
	return 0;
}

