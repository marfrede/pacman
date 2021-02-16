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

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Paccam(pWin, NULL), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048)
{
	createScene();
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
    //Menüauswahl
    //Bei Start

    pGame->update(dtime);

	if (gamemode == GameMode::FirstPerson) {
		Paccam.update();
	}
	else {
		Cam.update();
	}

	
}

void Application::draw()
{
	ShadowGenerator.generate(Models);

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models

	if (gamemode == GameMode::FirstPerson) {
        
        this->pGame->draw(Paccam);
        
		for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		{
			(*it)->draw(Paccam);
		}
	}
	else {
        
        this->pGame->draw(Cam);
        
		for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		{
			(*it)->draw(Cam);
		}
	}

	ShaderLightMapper::instance().deactivate();

	// 3. check once per frame for opengl errors
	GLenum Error = glGetError();
	assert(Error == 0);
}
void Application::end()
{
    this->pGame->end();

	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it) {
		delete* it;
	}
	this->Models.clear();
}

void Application::createScene()
{
	Matrix m, n;

	// SKY
	//pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	//pPhongShader = new PhongShader();
	//pModel->shader(pPhongShader, true);
	//pModel->shadowCaster(false);
	//Models.push_back(pModel);
    
    pGame = new Game();
    pGame->start(pWindow);

}
