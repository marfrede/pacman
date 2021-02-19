//
//  Freetype.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 19.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef Freetype_hpp
#define Freetype_hpp

#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "FreetypeShader.hpp"
#include <string>
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

class Freetype {
public:
    
    Freetype(const Camera Cam, GLFWwindow* window);
    
    void initFont();
    
    void RenderText(FreetypeShader &shader, std::string text, float x, float y, float scale, Vector color);
    
    unsigned int VAO, VBO;
    
    Camera Cam;
    GLFWwindow* window;
};

#endif /* Freetype_hpp */
