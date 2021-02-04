//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "Wall.h"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "ShadowMapGenerator.h"

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    typedef std::list<BaseModel*> WallList;
    typedef std::list<Ghost*> GhostList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();
protected:
	void createScene();
	void createNormalTestScene();
	void createShadowTestScene();
    Camera Cam;
    ModelList Models;
    WallList Walls;
    GhostList Ghosts;
    GLFWwindow* pWindow;
	BaseModel* pModel;
    Pacman* pPacman;
	ShadowMapGenerator ShadowGenerator;
};

#endif /* Application_hpp */
