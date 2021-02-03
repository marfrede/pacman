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
}

void Pacman::update(float dtime) {
    
    float forwardBackward = 0;
    float leftRight = 0;
    // move backwards
    /*
    if (glfwGetKey(this->pWindow, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_S) == GLFW_PRESS) {
        forwardBackward = -1.0f;
    }
    */
    // move forwards (even if both up/down pressed)
    if (glfwGetKey(this->pWindow, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_W) == GLFW_PRESS) {
        forwardBackward = 1.0f;
    }
    // move right
    if (glfwGetKey(this->pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_D) == GLFW_PRESS) {
        leftRight = -1.0f;
    }
    // move left (even if both left/rigth pressed)
    if (glfwGetKey(this->pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(this->pWindow, GLFW_KEY_A) == GLFW_PRESS) {
        leftRight = 1.0f;
    }
    
    this->steer(forwardBackward * dtime, leftRight * dtime);
    
}

void Pacman::steer(float forward, float leftRight) {
    /*
    float speed = 5;
    ForwardBackward *= speed;
    LeftRight *= speed;
     */
    
    if(leftRight == 0) {
        //Der Nutzer dreht nicht aktiv
        //Speichere aktuelle Position
        trnsfrm = this->transform();
        if(lR < 0) {
            float fl = floor(this->transform().left().X);
            double angleInRadians = atan2(this->transform().left().X, this->transform().left().Z);
            double angleInDegrees = (angleInRadians / M_PI) * 180.0;
            std::cout << "angleInDegrees: " << angleInDegrees << std::endl;
            std::cout << "FL: " << fl << std::endl;
            //Pacman muss aber weitergedreht werden
            //Drehe weiter nach links
            Matrix mTotal, mMovRot, mRot, mMov;
            mRot.rotationY(lR);
            mMov.translation(0, 0, 0);
            mMovRot = mMov * mRot;
            mTotal = this->transform() * mMovRot;
            this->transform(mTotal);
        } else if(lR > 0) {
            float cl = ceil(this->transform().left().X);
            std::cout << "LeftX: " << this->transform().left().X << std::endl;
            std::cout << "CL: " << cl << std::endl;
            //Pacman muss aber weitergedreht werden
            //Drehe weiter nach rechts
            Matrix mTotal, mMovRot, mRot, mMov;
            mRot.rotationY(lR);
            mMov.translation(0, 0, 0);
            mMovRot = mMov * mRot;
            mTotal = this->transform() * mMovRot;
            this->transform(mTotal);
        }
    } else if (leftRight != 0) {
        lR = leftRight;
    }
    
    std::cout << this->transform().left() << std::endl;
    std::cout << this->transform().right() << std::endl;
    
    Matrix mTotal, mMovRot, mRot, mMov;
    mRot.rotationY(leftRight);
    //std::cout << "m0 " << mRot.m00 << ":" << mRot.m01 << ":" << mRot.m02 << ":" << mRot.m03 << ":" << std::endl;
    //std::cout << "m1 " << mRot.m10 << ":" << mRot.m11 << ":" << mRot.m12 << ":" << mRot.m13 << ":" << std::endl;
    //std::cout << "m2 " << mRot.m20 << ":" << mRot.m21 << ":" << mRot.m22 << ":" << mRot.m23 << ":" << std::endl;
    //std::cout << "m3 " << mRot.m30 << ":" << mRot.m31 << ":" << mRot.m32 << ":" << mRot.m33 << ":" << std::endl;
    mMov.translation(forward, 0, 0);
    mMovRot = mMov * mRot;
    mTotal = this->transform() * mMovRot;
    this->transform(mTotal);
}
