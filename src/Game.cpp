//
//  Game.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 16.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#define TEXTURE_DIRECTORY "../../assets/texture/"
#else
#define ASSET_DIRECTORY "../assets/"
#define TEXTURE_DIRECTORY "../assets/texture/"
#endif

#include "Game.hpp"
#include "color.h"
#include "EgoCam.hpp"

Game::Game() {

}

BaseModel* Game::getPacman() {
	return this->pPacman;
}

void Game::update(float dtime) {

	for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
	{
		(*it)->update(dtime);
	}
	this->pField->update(dtime);

	this->pPacman->update(dtime);
	this->pPacman->adjustArrow(pField);

	//if(this->gamemode != GameMode::Debug) {
	this->checkGameOver();
	//}

}

void Game::draw(const Camera Cam)
{

	for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
	{
		(*it)->draw(Cam);
	}
	this->pField->draw(Cam);
	this->pPacman->draw(Cam);

}

void Game::createGameScene(GLFWwindow* pWindow) {

	pField = new Field();
	this->createGameModels(pWindow);

}

void Game::start(GLFWwindow* pWindow) {
	pField->reset();
	for (auto const& ghost : this->Ghosts) {
		ghost->reset();
	}
	pPacman->reset();
	this->gameOver = false;
}

void Game::checkGameOver() {

    if (!pField->pointsLeft()) {
        this->gameOver = true;
        return;
    }

    for (auto const& ghost : this->Ghosts) {
        Vector diff = pPacman->transform().translation() - ghost->transform().translation();
        if (diff.length() < 1) {
            this->gameOver = true;
            return;
        }
    }

    this->gameOver = false;

}

void Game::createGameModels(GLFWwindow* pWindow) {

	// GHOST RED
	//createGhost(pWindow, Color(242.0f / 255.0f, 5.0f / 255.0f, 3.0f / 255.0f), Color(1, 1, 1), 13, 14);

	//// GHOST ORANGE
	//createGhost(pWindow, Color(252.0f / 255.0f, 154.0f / 255.0f, 0.0f / 255.0f), Color(1, 1, 1), 16, 15);

	//// GHOST CYAN
	//createGhost(pWindow, Color(105.0f / 255.0f, 252.0f / 255.0f, 255.0f / 255.0f), Color(1, 1, 1), 13, 15);

	//// GHOST PINK
	//createGhost(pWindow, Color(252.0f / 255.0f, 154.0f / 255.0f, 153.0f / 255.0f), Color(1, 1, 1), 14, 15);

	//// GHOST WHITE
	//createGhost(pWindow, Color(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f), Color(0, 0, 0), 15, 15);

	// PACMAN
	createPacman(pWindow, Color(0, 0, 0), Color(0, 0, 0), 10, 15);

}

void Game::createPacman(GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ) {

	if (gamemode == GameMode::FirstPerson) {

		pPacman = new Pacman(posX, posZ);

	}
	else {
		pPacman = new Pacman(posX, posZ, ASSET_DIRECTORY "single-ghost-complete.dae", false);

		Model* ext = new Model(ASSET_DIRECTORY "single-ghost-ext.dae");
		ConstantShader* cShader = new ConstantShader();
		cShader->color(Color(1, 1, 1));
		ext->shader(cShader);
		pPacman->setExt(ext);
	}


	pPacman->setWindow(pWindow);
	pPacman->setField(pField);

	//Arrow
	Model* pModel = new Model(ASSET_DIRECTORY "arrow.dae", false);
	ConstantShader* pShader = new ConstantShader();
	pShader->color(Color(1.0f, 0, 0));
	pModel->shader(pShader, true);
	pPacman->setArrow(pModel);

}

void Game::createGhost(GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ) {

	Vector a = Vector(1, 0, 0.1f);
    float innerradius = 45;
    float outerradius = 70;

	// GHOST WHITE
	Ghost* g = new Ghost(posX, posZ, primary, ASSET_DIRECTORY "single-ghost-body.dae", false);
	Model* ext = new Model(ASSET_DIRECTORY "single-ghost-ext.dae");
	ConstantShader* cShader = new ConstantShader();
	cShader->color(secondary);
	ext->shader(cShader);
	g->setExt(ext);
	g->setWindow(pWindow);
	g->setField(pField);

	// point light
	PointLight* pl = new PointLight();
	pl->color(primary);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);
	g->setPointLight(pl);
    
	// spot light
	SpotLight* sl = new SpotLight();
	sl->color(primary);
    sl->direction(Vector(0, -1, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);
	g->setSpotLight(sl);
 
	Ghosts.push_back(g);

}

void Game::end() {
	for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it) {
		delete* it;
	}
	this->Ghosts.clear();
	this->pField->end();
}
