//
//  GameCharacter.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 03.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "GameCharacter.hpp"
#include "math.h"

GameCharacter::GameCharacter(int planeWidth, int planeDepth, int posX, int posZ, PhongShader* pPhongShader) : TriangleBoxModel(1, 1, 1)
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

void GameCharacter::update(float dtime) {
    
    this->steer(dtime);
    
}

void GameCharacter::steer(float dtime) {
    
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

bool GameCharacter::doCurrentAction(float dtime) {
    
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


void GameCharacter::rotate(float dtime, bool left) {
    
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

void GameCharacter::move(float dtime) {
    
    float movingUnits = movingSpeed * dtime;
    
    Matrix mTotal, mMov;
    mMov.translation(movingUnits, 0, 0);
    
    mTotal = this->transform() * mMov;
    this->transform(mTotal);
    
    moveUnits -= movingUnits;
    
}

bool GameCharacter::checkFront() {
    for (WallList::iterator it = Walls.begin(); it != Walls.end(); ++it)
    {
     //   (*it)->boundingBox().
    }
    return false;
}
