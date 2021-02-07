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
#include "Wall.h"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "ShadowMapGenerator.h"
#include "FieldSizeConstants.h"

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    typedef std::list<Ghost*> GhostList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();
protected:
    bool gameMode = true;
	void createScene();
	void createNormalTestScene();
	void createShadowTestScene();
    Camera Cam;
    EgoCam Paccam;
    Field* pField;
    ModelList Models;
    GhostList Ghosts;
    GLFWwindow* pWindow;
	BaseModel* pModel;
    Pacman* pPacman;
	ShadowMapGenerator ShadowGenerator;

	// temporary helper only for debugging
	std::string orientationToString(Orientation o) {
		switch (o)
		{
		case Orientation::North:
			return "North";
		case Orientation::East:
			return "East";
		case Orientation::West:
			return "West";
		default:
			return "South";
		}
	}
	std::string fieldTypeToString(FieldType ft) {
		switch (ft)
		{
		case FieldType::Wall:
			return "Wall";
		case FieldType::Point:
			return "Point";
		default:
			return "Free";
		}
	}
};

#endif /* Application_hpp */
