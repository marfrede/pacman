//
//  Pacman.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 01.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Pacman.hpp"
#include "math.h"

Pacman::Pacman(int posX, int posZ, const char* ModelFile, bool FitSize) : GameCharacter(posX, 0.5f, posZ) {
	Color yellow(249.0f / 250.0f, 250.0f / 250.0f, 6.0f / 250.0f);
	PhongShader* pPhongShader = new PhongShader();
	pPhongShader->ambientColor(yellow);
	pPhongShader->diffuseColor(yellow);
	pPhongShader->specularColor(yellow);
	this->shader(pPhongShader, true);
}

void Pacman::draw(const Camera Cam) {
    GameCharacter::draw(Cam);
    this->arrow->draw(Cam);
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

	if (!doCurrentAction(dtime)) {

		if (leftRight > 0) {
			angleToTurn = 90;
		}
		else if (leftRight < 0) {
			angleToTurn = -90;
		}
		else if (forward > 0 && checkFront()) {
			moveUnits = 1;
		}
		else {
			return;
		}
		doCurrentAction(dtime);

		// eat point
		std::pair<int, int> pos = this->getFieldPosition();
		this->pField->removePoint(pos.first, pos.second);
	}

	// pacman, print your infos:
	std::cout
		<< this->getFieldPosition().first << ", "
		<< this->getFieldPosition().second << "\t"
		<< fieldTypeToString(this->getFieldType()) << "\t"
		<< fieldTypeToString(this->getFieldTypeInFront()) << "\t"
		<< orientationToString(this->getOrientation()) << "\t"
		<< (this->checkFront() ? "can go" : "can not go") << std::endl;
}

void Pacman::adjustArrow(Field* pField) {
    
    Vector arrPos = this->arrow->transform().translation();
    Vector target = Vector(0,0,0);
    float closestDistance = 999999;
    
    for (auto const& point : pField->getPoints()) {
        Vector target = arrPos - point.second->transform().translation();
        if(target.length() < closestDistance) {
            closestDistance = target.length();
        }
        
    }
    
    //Target Vector on same height as arrow
    Vector targetH = Vector(target.X, this->arrow->transform().left().Y, target.Z);
    targetH.normalize();
    
    
    float angle = acos(targetH.dot(this->arrow->transform().left())); //Left weil das Model falsch ausgerichetet ist
    
    //Randbehandlung
    if (arrPos.Z > targetH.Z) {
        angle = 2 * M_PI - angle;
    }
    if (isnan(angle)) { // seltener Grenzfall bei gleicher Ausrichtung
        if (arrPos.Z < targetH.Z) angle = 0;
        else angle = M_PI;
    }
    
    Matrix mTotal, mRot, mRot2, mScale, mMov;
    mScale.scale(0.04f, 0.04f, 0.04f);
    mMov.translation(0, 3, 0);
    mRot.rotationY(angle);
    mRot2.rotationX((M_PI/2));
    mTotal = this->arrow->transform() * mScale * mMov * mRot * mRot2;
    
    this->arrow->transform(mTotal);
    
}

void Pacman::moveSubs() {

	GameCharacter::moveSubs();

	if (arrow) {
        
		Matrix mTotal, mMov, mTrans;
        mMov.translation(this->transform().forward()*0.25);;
        mTrans.translation(this->transform().translation());
        mTotal = mTrans * mMov;

		this->arrow->transform(mTotal);
         
	}

}


