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

Game::Game() {
	this->pField = nullptr;
	this->pPacman = nullptr;
}

Game::~Game() {
	this->end();
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
		if (spacePressed == false) {
			if (gamestatus == GameStatus::PLAYING) {
				gamestatus = GameStatus::PAUSE;
			}
			else if (gamestatus == GameStatus::PAUSE) {
				gamestatus = GameStatus::PLAYING;
			}
			else {
				this->start(pWindow);
			}
			spacePressed = true;
		}
	}
	else {
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
		pPacman = new Pacman(posX, posZ, primary, MODEL_DIRECTORY "pacman/pacman-body.dae", false); // with models
		pPacman->setExt(MODEL_DIRECTORY "pacman/pacman-ext.dae", secondary);
		lightColor = primary; // pacman primary color
	}
	else {
		pPacman = new Pacman(posX, posZ, Color(0, 0, 0)); // no models
		lightColor = Color(1, 1, 1); // pacman white color
	}

	// Arrow
	if (gamemode == GameMode::FirstPerson || gamemode == GameMode::Debug) {
		Color arrowColor(1.0f, 0.0f, 0.0f);
		pPacman->setArrow(MODEL_DIRECTORY "arrow/arrow.dae", arrowColor);
	}

	// Window and Field
	pPacman->setWindow(pWindow);
	pPacman->setField(pField);

	// Lights
	pPacman->setPointLight(Vector(0.35f, 0.35f, 0.35f), lightColor);
	pPacman->setSpotLight(70, 45, lightColor);
}

void Game::createGhost(GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ) {

	Ghost* g = new Ghost(posX, posZ, primary, MODEL_DIRECTORY "ghost/single-ghost-body.dae", false);
	g->setExt(MODEL_DIRECTORY "ghost/single-ghost-ext.dae", secondary);
	g->setWindow(pWindow);
	g->setField(pField);
	g->setPointLight(Vector(0.75f, 0, 0.75f), primary);
	g->setSpotLight(80, 45, primary);

	Ghosts.push_back(g);
}

void Game::end() {
	delete this->pPacman;
	for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it) {
		delete* it;
	}
	this->Ghosts.clear();
	delete this->pField;
}
