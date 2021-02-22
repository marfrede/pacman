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

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048)
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

	if (this->pGame->getGameStatus() == GameStatus::PLAYING) {
		pGame->update(dtime);
		moveCamera();
	}
	else {
		//hud->displayMenu(gs);
		//std::cout << "GAME IS OVER! (" << gameOverToString(go) << ")" << std::endl;
		//this->pGame->start(pWindow);
	}
}

void Application::draw()
{

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models

	Camera currentCam = Cam;
	this->pGame->draw(currentCam);

	if (this->pGame->getGameStatus() != GameStatus::PLAYING) {
		hud->displayMenu(this->pGame->getGameStatus());
	}

	ShaderLightMapper::instance().deactivate();

	// 3. check once per frame for opengl errors
	GLenum Error = glGetError();
	assert(Error == 0);
}

void Application::end()
{
	delete this->hud;
	delete this->pGame;
}

void Application::createScene()
{
	hud = new HUD();
	pGame = new Game();
	this->pGame->createGameScene(pWindow);
	moveCamera();
}

void Application::moveCamera() {
	if (pGame->getGameMode() == GameMode::FirstPerson) {
		Cam.setPosition(pGame->getPacman()->transform().translation());
		Cam.update(pGame->getPacman()->transform().translation() + pGame->getPacman()->transform().forward());
	}
	else if (pGame->getGameMode() == GameMode::ThirdPerson) {
		Cam.setPosition(this->pGame->getPacman()->transform().translation() + this->pGame->getPacman()->transform().backward() * 5 + this->pGame->getPacman()->transform().up() * 10);
		Cam.update(pGame->getPacman()->transform().translation());
	}
	else {
		Cam.update();
	}
}
