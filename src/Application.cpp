//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "wall.h"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "model.h"
#include "ShaderLightmapper.h"


#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#define TEXTURE_DIRECTORY "../../assets/texture/"
#else
#define ASSET_DIRECTORY "../assets/"
#define TEXTURE_DIRECTORY "../assets/texture/"
#endif

using namespace std;

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048)
{
	createScene();
	//createNormalTestScene();
	//createShadowTestScene();


}
void Application::start()
{
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime)
{
	Cam.update();
    pPacman->update(dtime);
    for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
    {
        (*it)->update(dtime);
    }
}

void Application::draw()
{
	ShadowGenerator.generate(Models);

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models
	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
	{
		(*it)->draw(Cam);
	}
    for (WallList::iterator it = Walls.begin(); it != Walls.end(); ++it)
    {
        (*it)->draw(Cam);
    }
    for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
    {
        (*it)->draw(Cam);
    }
	ShaderLightMapper::instance().deactivate();

	// 3. check once per frame for opengl errors
	GLenum Error = glGetError();
	assert(Error == 0);
}
void Application::end()
{
	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		delete* it;

	Models.clear();
    
    for (WallList::iterator it = Walls.begin(); it != Walls.end(); ++it)
        delete* it;

    Walls.clear();
    
    for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
        delete* it;

    Ghosts.clear();
}

void Application::createScene()
{
	Matrix m, n;
	ConstantShader* pConstShader;
	PhongShader* pPhongShader;

	// SKY
	//pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	//pPhongShader = new PhongShader();
	//pModel->shader(pPhongShader, true);
	//pModel->shadowCaster(false);
	//Models.push_back(pModel);

	// CHEQUERED PLAYING FIELD
	int planeWidth = 30, planeDepth = 33;
	pModel = new LinePlaneModel((float) planeWidth, (float) planeDepth, (float) planeWidth, (float) planeDepth);
	pConstShader = new ConstantShader();
	pConstShader->color(Color(1, 0, 0));
	pModel->shader(pConstShader, true);
	Models.push_back(pModel);
    
    // FIELD
    pModel = new TrianglePlaneModel(30, 33, 10, 10);
    pPhongShader = new PhongShader();
    pPhongShader->ambientColor(Color(0.2f,0.2f,0.2f));
    pPhongShader->diffuseColor(Color(1.0f,1.0f,1.0f));
    pPhongShader->diffuseTexture(Texture::LoadShared(TEXTURE_DIRECTORY "dirtyBricks_C_01.dds"));
    pModel->shader(pPhongShader, true);
    Models.push_back( pModel );

	// WALLS
	// 1. set wall padding and height
	float padding = 0.0f;
	float wallHeight = 1;

	// 2. set wall positions
	// map origin position (x, z) to expansion (width, depth)
	map<pair<float, float>, pair<float, float>> walls{

		// inner walls
		{{3,3}, {4,3}},
		{{8,3}, {5,3}},
		{{17,3}, {5,3}},
		{{23,3}, {4,3}},
		{{3,7}, {4,2}},
		{{8,7}, {2,8}},
		{{11,7}, {8,2}},
		{{20,7}, {2,8}},
		{{23,7}, {4,2}},
		{{14,9}, {2,3}}, // {{14,8}, {2,4}},
		{{10,10}, {3,2}}, // {{9,10}, {4,2}},
		{{17,10}, {3,2}}, // {{17,10}, {4,2}},
		{{11,13}, {3,2}},
		{{16,13}, {3,2}},
		{{11,16}, {8,2}},
		{{11,15}, {2,1}}, // {{10,15}, {2,3}},
		{{17,15}, {2,1}}, // {{16,15}, {2,3}},
		{{8,16}, {2,5}},
		{{20,16}, {2,5}},
		{{11,19}, {8,2}},
		{{14,21}, {2,3}}, // {{14,20}, {2,4}},
		{{3,22}, {4,2}},
		{{8,22}, {5,2}},
		{{17,22}, {5,2}},
		{{23,22}, {4,2}},
		{{5,24}, {2,3}}, // {{5,23}, {2,4}},
		{{23,24}, {2,3}}, // {{23,23}, {2,4}},
		{{8,25}, {2,3}}, // {{8,25}, {2,4}},
		{{11,25}, {8,2}},
		{{20,25}, {2,3}}, // {{20,25}, {2,4}},
		{{14,27}, {2,3}}, // {{14,26}, {2,4}},
		{{3,28}, {10,2}},
		{{17,28}, {10,2}},

		// outer walls
		{{1,1}, {28,1}},
		{{1,2}, {1,8}}, // {{1,1}, {1,10}},
		{{28,2}, {1,8}}, // {{28,1}, {1,10}},
		{{1,10}, {6,1}},
		{{23,10}, {6,1}},
		{{6,11}, {1,3}}, // {{6,10}, {1,5}},
		{{23,11}, {1,3}}, // {{23,10}, {1,5}},
		{{1,14}, {6,1}},
		{{23,14}, {6,1}},
		{{1,16}, {6,1}},
		{{23,16}, {6,1}},
		{{6,17}, {1,3}}, // {{6,16}, {1,5}},
		{{23,17}, {1,3}}, // {{23,16}, {1,5}},
		{{1,20}, {6,1}},
		{{23,20}, {6,1}},
		{{1,21}, {1,10}}, // {{1,20}, {1,12}},
		{{28,21}, {1,10}}, // {{28,20}, {1,12}},
		{{1,31}, {28,1}},
		// outer / inner walls
		{{14,2}, {2,4}},
		{{2,25}, {2,2}},
		{{26,25}, {2,2}},
	};

	// 3. set shader equally for all walls
	pPhongShader = new PhongShader();
	pPhongShader->ambientColor(Color(0.2f, 0.2f, 0.2f)); // normal grey
	//pPhongShader->ambientColor(Color(0.14902f, 0.15294f, 0.8f)); // pacman blue wall color
	pPhongShader->diffuseColor(Color(1.0f, 1.0f, 1.0f));
	pPhongShader->specularColor(Color(1.0f, 1.0f, 1.0f));
	//pPhongShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "smiley.png"));
	pPhongShader->diffuseTexture(Texture::LoadShared(TEXTURE_DIRECTORY "PaintedPlaster014_4K_Color.jpg"));

	// 4. make walls
	for (auto const& wall : walls)
	{
		Models.push_back(
			new Wall(planeWidth, planeDepth,
				wall.second.first, // width
				wallHeight,
				wall.second.second, // depth
				wall.first.first, // posX
				wall.first.second, // posY
				pPhongShader, padding)
		);
	}
    
    //GHOSTS
    pPhongShader = new PhongShader();
    //pPhongShader->ambientColor(Color(0.14902f, 0.15294f, 0.8f)); // pacman blue wall color
    pPhongShader->ambientColor(Color(0.2f, 0.2f, 0.2f));
    pPhongShader->diffuseColor(Color(1.0f, 1.0f, 1.0f));
    pPhongShader->specularColor(Color(1.0f, 1.0f, 1.0f));
    Ghost* g = new Ghost(planeWidth, planeDepth, 5, 10, pPhongShader);
    g->setWindow(pWindow);
    g->setWalls(Walls);
    Ghosts.push_back(g);
    g = new Ghost(planeWidth, planeDepth, 0, 0, pPhongShader);
    g->setWindow(pWindow);
    g->setWalls(Walls);
    Ghosts.push_back(g);
    g = new Ghost(planeWidth, planeDepth, 1, 5, pPhongShader);
    g->setWindow(pWindow);
    g->setWalls(Walls);
    Ghosts.push_back(g);
    g = new Ghost(planeWidth, planeDepth, 2, 7, pPhongShader);
    g->setWindow(pWindow);
    g->setWalls(Walls);
    Ghosts.push_back(g);
    g = new Ghost(planeWidth, planeDepth, 3, 8, pPhongShader);
    g->setWindow(pWindow);
    g->setWalls(Walls);
    Ghosts.push_back(g);
    
    //PACMAN
    pPhongShader = new PhongShader();
    //pPhongShader->ambientColor(Color(0.14902f, 0.15294f, 0.8f)); // pacman blue wall color
    pPhongShader->ambientColor(Color(0.2f, 0.2f, 0.2f));
    pPhongShader->diffuseColor(Color(1.0f, 1.0f, 1.0f));
    pPhongShader->specularColor(Color(1.0f, 1.0f, 1.0f));
    pPacman = new Pacman(planeWidth, planeDepth, 0, 0, pPhongShader);
    pPacman->setWindow(pWindow);
    pPacman->setWalls(Walls);
    Models.push_back(pPacman);
}

void Application::createNormalTestScene()
{
	pModel = new LinePlaneModel(10, 10, 10, 10);
	ConstantShader* pConstShader = new ConstantShader();
	pConstShader->color(Color(0, 0, 0));
	pModel->shader(pConstShader, true);
	// add to render list
	Models.push_back(pModel);


	pModel = new Model(ASSET_DIRECTORY "cube.obj", false);
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);


}

void Application::createShadowTestScene()
{
	pModel = new Model(ASSET_DIRECTORY "shadowcube.obj", false);
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);

	pModel = new Model(ASSET_DIRECTORY "bunny.dae", false);
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);

	// directional lights
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0, -1, -1));
	dl->color(Color(0.5, 0.5, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	SpotLight* sl = new SpotLight();
	sl->position(Vector(2, 2, 0));
	sl->color(Color(0.5, 0.5, 0.5));
	sl->direction(Vector(-1, -1, 0));
	sl->innerRadius(10);
	sl->outerRadius(13);
	sl->castShadows(true);
	ShaderLightMapper::instance().addLight(sl);
}
