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

void Pacman::moveSubs() {
    
    std::cout << "Pacman moveSubs" << std::endl;
    GameCharacter::moveSubs();
    
    std::cout << this->arrow << std::endl;
    if(arrow) {
        Matrix mTotal, mMov, mScale, mRot;
        mMov.translation(0.5, 0.25, 0);
        mScale.scale(0.05f, 0.05f, 0.05f);
        mRot.rotationY(90);
        
        mTotal = this->transform() * mMov * mScale * mRot;

        this->arrow->transform(mTotal);
    }
    
}


