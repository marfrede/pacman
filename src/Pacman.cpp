//
//  Pacman.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 01.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Pacman.hpp"
#include "math.h"

Pacman::Pacman(int planeWidth, int planeDepth, int posX, int posZ, PhongShader* pPhongShader) : TriangleBoxModel(1, 1, 1)
{
    this->shader(pPhongShader, true);
    Matrix t;
    t.translation(
        posX,
        1,
        posZ
    );
    this->transform(t);
    angleToTurn = 0;
}

void Pacman::update(float dtime) {
    
    float forwardBackward = 0;
    float leftRight = 0;

    if (glfwGetKey(this->pWindow, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_W) == GLFW_PRESS) {
        forwardBackward = 1.0f;
    }
    // move right
    if (glfwGetKey(this->pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_D) == GLFW_PRESS) {
        leftRight = 1.0f;
    }
    // move left (even if both left/rigth pressed)
    if (glfwGetKey(this->pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_A) == GLFW_PRESS) {
        leftRight = -1.0f;
    }
    
    this->steer(forwardBackward * dtime, leftRight * dtime, dtime);
    
}

void Pacman::steer(float forward, float leftRight, float dtime) {
    
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

bool Pacman::doCurrentAction(float dtime) {
    
    //Check rotation
    if(angleToTurn > 0) {
        //Führe Rotation nach rechts aus

        rotate(dtime, false);
        
        return true;
        
    } else if(angleToTurn < 0) {
        //Führe Rotation nach links aus
        
        rotate(dtime, true);
        
        return true;
        
    } else if(moveUnits > 0) { //Check movement
        
        move(dtime);
        
        return true;
        
    } else {
        return false;
    }
}


void Pacman::rotate(float dtime, bool left) {
    
    float rotateAngle = rotateSpeed * dtime;
    float angle = rotateAngle * (2*M_PI)/360;
    
    if(left) {
        
        angle *= -1;
        
        if(angleToTurn-rotateAngle > 0) {
            angle = angleToTurn * (2*M_PI/360);
            rotateAngle = angleToTurn;
        }
        
    } else {
        
        if(angleToTurn-rotateAngle < 0) {
            angle = angleToTurn * (2*M_PI/360);
            rotateAngle = angleToTurn;
        }
        
    }
    
    Matrix mTotal, mRot;
    mRot.rotationY(-angle);
    
    mTotal = this->transform() * mRot;
    this->transform(mTotal);
    
    if(left) {
        angleToTurn += rotateAngle;
    } else {
        angleToTurn -= rotateAngle;
    }
    
}

void Pacman::move(float dtime) {
    
    float movingUnits = movingSpeed * dtime;
    
    Matrix mTotal, mMov;
    mMov.translation(movingUnits, 0, 0);
    
    mTotal = this->transform() * mMov;
    this->transform(mTotal);
    
    moveUnits -= movingUnits;
    
}
