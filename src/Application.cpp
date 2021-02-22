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

using namespace std;

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), Paccam(pWin, NULL), pModel(NULL), ShadowGenerator(2048, 2048)
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
    pGame->manageInputs(pWindow);
    
    if(this->pGame->getGameStatus() == GameStatus::PLAYING) {
        pGame->update(dtime);
        moveCamera();
    } else {
        //hud->displayMenu(gs);
    }
}

void Application::draw()
{
	ShadowGenerator.generate(Models);

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models

    this->pGame->draw(Cam);
    
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
    {
        (*it)->draw(Cam);
    }
    
    if(this->pGame->getGameStatus() != GameStatus::PLAYING) {
        hud->displayMenu(this->pGame->getGameStatus());
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
    
    hud = new HUD();
    pGame = new Game();
    this->pGame->createGameScene(pWindow);
    this->Paccam.setObj(pGame->getPacman());
    
    moveCamera();
}

void Application::moveCamera() {
    if (pGame->getGameMode() == GameMode::FirstPerson) {
        Cam.setPosition(pGame->getPacman()->transform().translation());
        Cam.update(pGame->getPacman()->transform().translation() + pGame->getPacman()->transform().forward());
    } else if (pGame->getGameMode() == GameMode::ThirdPerson) {
        Cam.setPosition(this->pGame->getPacman()->transform().translation() + this->pGame->getPacman()->transform().backward() * 5 + this->pGame->getPacman()->transform().up() * 10);
        Cam.update(pGame->getPacman()->transform().translation());
    } else {
        Cam.update();
    }
}
