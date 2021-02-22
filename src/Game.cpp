//
//  Game.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 16.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

#define MODEL_DIRECTORY ASSET_DIRECTORY "models/"
#define TEXTURE_DIRECTORY ASSET_DIRECTORY  "texture/"

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
	this->pPacman->adjustArrow(pField, this->gamemode);

	if (this->gamemode != GameMode::Debug) {
		this->checkGameOver();
	}

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

	if (gamemode == GameMode::FirstPerson) {
		pField = new Field(5.5f);
	}
	else {
		pField = new Field(1.5f);
	}

	this->createGameModels(pWindow);

}

void Game::start(GLFWwindow* pWindow) {
	pField->reset();
	for (auto const& ghost : this->Ghosts) {
		ghost->reset();
	}
	pPacman->reset();
    this->gamestatus = GameStatus::PLAYING;
}

void Game::manageInputs(GLFWwindow* pWindow) {
    if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if(spacePressed == false) {
            if(gamestatus == GameStatus::PLAYING) {
                gamestatus = GameStatus::PAUSE;
            } else if (gamestatus == GameStatus::PAUSE) {
                gamestatus = GameStatus::PLAYING;
            } else {
                this->start(pWindow);
            }
            spacePressed = true;
        }
    } else {
        spacePressed = false;
    }
}

GameStatus Game::checkGameOver() {

	if (!pField->pointsLeft()) {
        this->gamestatus = GameStatus::ALL_POINTS_COLLECTED;
        
	}

	for (auto const& ghost : this->Ghosts) {
		Vector diff = pPacman->transform().translation() - ghost->transform().translation();
		if (diff.length() < 1) {
            this->gamestatus = GameStatus::CATCHED_BY_GHOST;
		}
	}

    return this->gamestatus;
    
}

void Game::createGameModels(GLFWwindow* pWindow) {

	// GHOST RED
	createGhost(pWindow, Color(242.0f / 255.0f, 5.0f / 255.0f, 3.0f / 255.0f), Color(1, 1, 1), 13, 14);

	// GHOST ORANGE
	createGhost(pWindow, Color(252.0f / 255.0f, 154.0f / 255.0f, 0.0f / 255.0f), Color(1, 1, 1), 16, 15);

	// GHOST CYAN
	createGhost(pWindow, Color(105.0f / 255.0f, 252.0f / 255.0f, 255.0f / 255.0f), Color(1, 1, 1), 13, 15);

	// GHOST PINK
	createGhost(pWindow, Color(252.0f / 255.0f, 154.0f / 255.0f, 153.0f / 255.0f), Color(1, 1, 1), 14, 15);

	// GHOST WHITE
	createGhost(pWindow, Color(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f), Color(0, 0, 0), 15, 15);

	// PACMAN
	createPacman(pWindow, Color(255.0f / 255.0f, 200.0f / 255.0f, 0 / 255.0f), Color(0, 0, 0), 10, 15);

}

void Game::createPacman(GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ) {
	// std::cout << "PACMAN " << std::endl;

	Color lightColor;
	if (gamemode != GameMode::FirstPerson) {
		// Pacman with Model
		pPacman = new Pacman(posX, posZ, primary, MODEL_DIRECTORY "pacman/pacman-body.dae", false);
		Model* ext = new Model(MODEL_DIRECTORY "pacman/pacman-ext.dae");
		ConstantShader* cShader = new ConstantShader();
		cShader->color(secondary);
		ext->shader(cShader);
		pPacman->setExt(ext);
		// Pacman primary color
		lightColor = primary;
	}
	else {
		// Pacman without Model
		pPacman = new Pacman(posX, posZ, Color(0, 0, 0));
		// Pacman white color
		lightColor = Color(1, 1, 1);
	}

	// Arrow
	if (gamemode == GameMode::FirstPerson || gamemode == GameMode::Debug) {
		Model* arrow = new Model(MODEL_DIRECTORY "arrow/arrow.dae", false);
		ConstantShader* pShader = new ConstantShader();
		pShader->color(Color(1.0f, 0, 0));
		arrow->shader(pShader, true);
		pPacman->setArrow(arrow);
	}

	// Window and Field
	pPacman->setWindow(pWindow);
	pPacman->setField(pField);

	// Light 2
	Vector a = Vector(0.35f, 0.35f, 0.35f);
	float innerradius = 45;
	float outerradius = 70;

	PointLight* pl = new PointLight();
	pl->color(lightColor);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);
	pPacman->setPointLight(pl);

	// spot light
	SpotLight* sl = new SpotLight();
	sl->color(lightColor);
	sl->direction(Vector(0, -1, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);
	pPacman->setSpotLight(sl);
}

void Game::createGhost(GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ) {

	Vector a = Vector(0.75f, 0, 0.75f);
	float innerradius = 45;
	float outerradius = 80;

	// GHOST WHITE
	Ghost* g = new Ghost(posX, posZ, primary, MODEL_DIRECTORY "ghost/single-ghost-body.dae", false);
	Model* ext = new Model(MODEL_DIRECTORY "ghost/single-ghost-ext.dae");
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
