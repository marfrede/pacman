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
#include "field.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "GameMode.h"
#include "Game.hpp"
#include "Wall.h"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "ShadowMapGenerator.h"
#include "FieldSizeConstants.h"
#include "HUD.hpp"

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();
protected:
	void createScene();
    void moveCamera();
    HUD* hud;
    
    Game* pGame;
    
    Camera Cam = NULL;
    
    ModelList Models;
    GLFWwindow* pWindow;
    BaseModel* pModel;
	ShadowMapGenerator ShadowGenerator;
};

#endif /* Application_hpp */
