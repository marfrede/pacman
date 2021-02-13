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
	pField->update(dtime);

	pPacman->update(dtime);

	// pacman, print your infos:
	std::cout
		<< pPacman->getFieldPosition().first << ", "
		<< pPacman->getFieldPosition().second << "\t"
		<< this->fieldTypeToString(pPacman->getFieldType()) << "\t"
		<< this->fieldTypeToString(pPacman->getFieldTypeInFront()) << "\t"
		<< this->orientationToString(pPacman->getOrientation()) << "\t"
		<< (pPacman->checkFront() ? "can go" : "can not go") << std::endl;

	if (gameMode) {
		Paccam.update();
	}
	else {
		Cam.update();
	}

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

	if (gameMode) {
		for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		{
			(*it)->draw(Paccam);
		}
		for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
		{
			(*it)->draw(Paccam);
		}
		this->pField->draw(Paccam);
		for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		{
			(*it)->draw(Paccam);
		}
		for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
		{
			(*it)->draw(Paccam);
		}
	}
	else {
		for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		{
			(*it)->draw(Cam);
		}
		for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
		{
			(*it)->draw(Cam);
		}
		this->pField->draw(Cam);
		for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		{
			(*it)->draw(Cam);
		}
		for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it)
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


	for (GhostList::iterator it = Ghosts.begin(); it != Ghosts.end(); ++it) {
		delete* it;
	}
	this->Ghosts.clear();

	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it) {
		delete* it;
	}
	this->Models.clear();

	this->pField->end();
}

void Application::createScene()
{
	Matrix m, n;
	PhongShader* pPhongShader;

	Color c = Color(1.0f, 0.7f, 1.0f);
	Vector a = Vector(1, 0, 0.1f);
	float innerradius = 45;
	float outerradius = 90;

	// SKY
	//pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	//pPhongShader = new PhongShader();
	//pModel->shader(pPhongShader, true);
	//pModel->shadowCaster(false);
	//Models.push_back(pModel);
    
    std::cout << "Field" << std::endl;
	pField = new Field();
    std::cout << "Field End" << std::endl;

	//GHOST RED
	c = Color(242.0f / 255.0f, 5.0f / 255.0f, 3.0f / 255.0f);
	Ghost* g = new Ghost(13, 14, c, ASSET_DIRECTORY "single-ghost-complete.dae", false);
	g->setWindow(pWindow);
	g->setField(this->pField);
	// point lights
	PointLight* pl = new PointLight();
	pl->color(c);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);
	g->setPointLight(pl);
	// spot lights
	SpotLight* sl = new SpotLight();
	sl->color(Color(242.0f / 255.0f, 5.0f / 255.0f, 3.0f / 255.0f));
	sl->direction(Vector(0, -1, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);
	g->setSpotLight(sl);
	Ghosts.push_back(g);

	//GHOST ORANGE
	c = Color(252.0f / 255.0f, 154.0f / 255.0f, 0.0f / 255.0f);
	g = new Ghost(12, 15, c, ASSET_DIRECTORY "single-ghost-complete.dae", false);
	g->setWindow(pWindow);
	g->setField(this->pField);
	// point lights
	pl = new PointLight();
	pl->color(c);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);
	g->setPointLight(pl);
	// spot lights
	sl = new SpotLight();
	sl->color(Color(252.0f / 255.0f, 154.0f / 255.0f, 0.0f / 255.0f));
	sl->direction(Vector(0, -1, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);
	g->setSpotLight(sl);
	Ghosts.push_back(g);

	// GHOST CYAN
	c = Color(105.0f / 255.0f, 252.0f / 255.0f, 255.0f / 255.0f);
	g = new Ghost(13, 15, c, ASSET_DIRECTORY "single-ghost-complete.dae", false);
	g->setWindow(pWindow);
	g->setField(this->pField);
	// point lights
	pl = new PointLight();
	pl->color(c);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);
	g->setPointLight(pl);
	// spot lights
	sl = new SpotLight();
	sl->color(Color(105.0f / 255.0f, 252.0f / 255.0f, 255.0f / 255.0f));
	sl->direction(Vector(0, -1, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);
	g->setSpotLight(sl);
	Ghosts.push_back(g);

	// GHOST PINK
	c = Color(252.0f / 255.0f, 154.0f / 255.0f, 153.0f / 255.0f);
	g = new Ghost(14, 15, c, ASSET_DIRECTORY "single-ghost-complete.dae", false);
	g->setWindow(pWindow);
	g->setField(this->pField);
	// point lights
	pl = new PointLight();
	pl->color(c);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);
	g->setPointLight(pl);
	// spot lights
	sl = new SpotLight();
	sl->color(Color(252.0f / 255.0f, 154.0f / 255.0f, 153.0f / 255.0f));
	sl->direction(Vector(0, -1, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);
	g->setSpotLight(sl);
	Ghosts.push_back(g);

	// GHOST WHITE
	c = Color(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	g = new Ghost(15, 15, c, ASSET_DIRECTORY "single-ghost-complete.dae", false);
	g->setWindow(pWindow);
	g->setField(this->pField);
	// point light
	pl = new PointLight();
	pl->color(c);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);
	g->setPointLight(pl);
	// spot light
	sl = new SpotLight();
	sl->color(Color(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
	sl->direction(Vector(0, -1, 0));
	sl->innerRadius(innerradius);
	sl->outerRadius(outerradius);
	ShaderLightMapper::instance().addLight(sl);
	g->setSpotLight(sl);
	Ghosts.push_back(g);

    std::cout << "Pacman" << std::endl;
	//PACMAN
	pPacman = new Pacman((int)(PLANE_WIDTH / 2.0f), (int)(PLANE_DEPTH / 2.0f), ASSET_DIRECTORY "single-ghost-complete.dae", false);
	pPacman->setWindow(pWindow);
	pPacman->setField(this->pField);

    pModel = new Model(ASSET_DIRECTORY "arrow.dae", false);
    ConstantShader* pShader = new ConstantShader();
    pShader->color(Color(1.0f, 0, 0));
    pModel->shader(pShader, true);
    //Matrix mTotal = m.translation(0, 1, 0);
    //pModel->transform();
    pPacman->setArrow(pModel);
    Models.push_back(pModel);

	Paccam.setObj(pPacman);
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
