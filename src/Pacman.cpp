//
//  Pacman.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 01.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Pacman.hpp"
#include "math.h"

Pacman::Pacman(int planeWidth, int planeDepth, int posX, int posZ, PhongShader* pPhongShader) : GameCharacter(planeWidth, planeDepth, posX, posZ, pPhongShader) {
}

void Pacman::update(float dtime) {
    
    this->steer(dtime);
    
    /*
    if(paccam) {
        this->movePaccam();
    }
     */
    
}

void Pacman::steer(float dtime) {
    // std::cout << "Pacman steer!" << std::endl;
    
    float forward = 0;
    float leftRight = 0;
    
    if (glfwGetKey(this->pWindow, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_W) == GLFW_PRESS) {
        forward = 1.0f;
    }
    // move right
    if (glfwGetKey(this->pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_D) == GLFW_PRESS) {
        leftRight = 1.0f;
    }
    // move left (even if both left/rigth pressed)
    if (glfwGetKey(this->pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_A) == GLFW_PRESS) {
        leftRight = -1.0f;
    }
    
    if(!doCurrentAction(dtime)) {
        
        if(leftRight > 0) {
            angleToTurn = 90;
        } else if(leftRight < 0) {
            angleToTurn = -90;
        } else if(forward > 0) {
            moveUnits = 1;
        } else {
            return;
        }
        
        doCurrentAction(dtime);
        
    }
}

/*
void Pacman::movePaccam() {
    paccam->setPosition(this->transform().translation());
    paccam->setTarget(this->transform().translation() + this->transform().forward());
    //Vector Pos = position(); //m_Position + m_Panning + m_Zoom + m_Rotation;
    //Vector Target = target(); //m_Target + m_Panning;
    //m_ViewMatrix.lookAt(Target, m_Up, Pos);
}
*/
