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
#include "model.h"
#include "ShaderLightmapper.h"


#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#define TEXTURE_DIRECTORY "../../assets/texture/"
#else
#define ASSET_DIRECTORY "../assets/"
#define TEXTURE_DIRECTORY "../assets/texture/"
#endif


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
}

void Application::createScene()
{
	Matrix m, n;
	ConstantShader* pConstShader;
	PhongShader* pPhongShader;

	// SKY
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	pPhongShader = new PhongShader();
	pModel->shader(pPhongShader, true);
	pModel->shadowCaster(false);
	Models.push_back(pModel);

	// CHEQUERED PLAYING FIELD
	int planeWidth = 30, planeDepth = 33;
	pModel = new LinePlaneModel((float) planeWidth, (float) planeDepth, (float) planeWidth, (float) planeDepth);
	pConstShader = new ConstantShader();
	pConstShader->color(Color(1, 0, 0));
	pModel->shader(pConstShader, true);
	Models.push_back(pModel);

	// WALLS
	pPhongShader = new PhongShader();
	//pPhongShader->ambientColor(Color(0.14902f, 0.15294f, 0.8f)); // pacman blue wall color
	pPhongShader->ambientColor(Color(0.2f, 0.2f, 0.2f));
	pPhongShader->diffuseColor(Color(1.0f, 1.0f, 1.0f));
	pPhongShader->specularColor(Color(1.0f, 1.0f, 1.0f));
	pPhongShader->diffuseTexture(Texture::LoadShared(TEXTURE_DIRECTORY "PaintedPlaster014_4K_Color.jpg"));
	// Models.push_back(new Wall(planeWidth, planeDepth, length, height, depth, posX, posY, pPhongShader));
	Models.push_back(new Wall(planeWidth, planeDepth, 4, 4, 2, 3, 3, pPhongShader));
	Models.push_back(new Wall(planeWidth, planeDepth, 5, 4, 2, 8, 3, pPhongShader));
	Models.push_back(new Wall(planeWidth, planeDepth, 4, 4, 2, 3, 6, pPhongShader));
	//Models.push_back(new Wall(planeWidth, planeDepth, 4, 4, 1, 8, 0, pPhongShader));

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