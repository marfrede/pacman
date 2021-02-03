//
//  Pacman.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 01.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef Pacman_hpp
#define Pacman_hpp

#include "TriangleBoxModel.h"
#include "PhongShader.h"
#include <stdio.h>

class Pacman : public TriangleBoxModel
{
public:
    /**
    * create a TriangleBoxModel at given position
    * @param planeWidth playing field width
    * @param planeDepth playing field depth
    * @param width in x-direction of the wall
    * @param height in y-direction of the wall
    * @param depth in z-direction of the wall
    * @param posX (min:0, max:planeWidth) x position on the planeModel
    * @param posZ (min:0, max:planeDepth) z position on the planeModel
    * @param pPhongShader to use for the walls
    */
    Pacman(int planeWidth, int planeDepth, int posX, int posZ, PhongShader* pPhongShader);
    
    void setWindow(GLFWwindow* window) {pWindow = window;}
    void update(float dtime);
    
    void steer(float forward, float leftRight, float dtime);
    void rotate(float dtime, bool left);
    void move(float dtime);
    bool doCurrentAction(float dtime);
    
    
    
private:
    
    GLFWwindow* pWindow;
    
    //STEERING
    //Rotation
    float angleToTurn;
    float rotateSpeed = 200;
    //Moving
    float moveUnits;
    float movingSpeed = 3;
    
    
};

#endif /* Pacman_hpp */
