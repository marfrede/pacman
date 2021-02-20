//
//  Pacman.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 01.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Pacman.hpp"
#include "math.h"

Pacman::Pacman(int posX, int posZ, Color c, const char* ModelFile, bool FitSize) : GameCharacter(posX, 0.5f, posZ, ModelFile, FitSize) {
	this->init(c);
}

Pacman::Pacman(int posX, int posZ) : GameCharacter(posX, 0.5f, posZ) {
	this->init();
}

void Pacman::init() {
    ConstantShader* pShader = new ConstantShader();
    pShader->color(Color(0,0,0));
    this->shader(pShader, true);
    this->rotateSpeed = 350;
    this->movingSpeed = 3.2;
    this->arrow = nullptr;
}

void Pacman::init(Color c) {
    ConstantShader* pShader = new ConstantShader();
    pShader->color(c);
    this->shader(pShader, true);
	this->rotateSpeed = 350;
	this->movingSpeed = 3.2;
	this->arrow = nullptr;
}

void Pacman::setArrow(BaseModel* arrow) {
	this->arrow = arrow;
}

void Pacman::draw(const Camera Cam) {
	GameCharacter::draw(Cam);
	if (this->arrow) {
		this->arrow->draw(Cam);
	}
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
	}

	// pacman, print your infos:
	//std::cout
		//<< this->getFieldPosition().first << ", "
		//<< this->getFieldPosition().second << "\t"
		//<< fieldTypeToString(this->getFieldType()) << "\t"
		//<< fieldTypeToString(this->getFieldTypeInFront()) << "\t"
		//<< orientationToString(this->getOrientation()) << "\t"
		//<< (this->checkFront() ? "can go" : "can not go") << std::endl;
}

void Pacman::adjustArrow(Field* pField, GameMode gamemode) {
	if (!this->arrow) {
		return;
	}

	Vector pacPos = this->transform().translation();
	Vector arrPos = this->arrow->transform().translation();
	Vector target = this->pField->closestPointPos(arrPos); // Here, the arrow position and not the Pacman position is used to determine the closest point, as the arrow is always moved (translation) a bit BEFORE Pacman (depending on Pacman's current orientation). This means that the arrow always points to the point Pacman is already looking at in case there are several points in the same distance. 
	Vector dirToTarget = pacPos - target;
	dirToTarget.Y = 0; // just horiztontal direction important! 
	dirToTarget.normalize();

	float angle = acos(dirToTarget.dot(this->arrow->transform().left())); //direction left instead of forward used because the arrow 3D model  is misaligned (ponting left)
	if (pacPos.Z < target.Z && abs(pacPos.Z - target.Z) > 0.1f) {
		// scalar calcs smallest angle
		angle = -angle;
	}
	if (isnan(angle)) { // Limit case with alignment (scalar = 0)
		if (pacPos.Z < target.Z) angle = -M_PI / 2.0f;
		else angle = M_PI / 2.0f;
	}

	Matrix mTotal, mRotY, mRotX, mScale, mMov;
	mScale.identity();
	if(gamemode != GameMode::Debug) {
		mScale.scale(0.04f, 0.04f, 0.04f);
	}
	mMov.translation(0, 3, 0);
	mRotY.rotationY(angle);
	mRotX.rotationX((M_PI / 2));
	mTotal = this->arrow->transform() * mScale * mMov * mRotY * mRotX;

	this->arrow->transform(mTotal);
}

void Pacman::moveSubs() {
    std::cout << "Pacman move subs" << std::endl;
    
	GameCharacter::moveSubs();

	if (!this->arrow) {
		return;
	}

	Matrix mTotal, mMov, mTrans;
	mMov.translation(this->transform().forward() * 0.25);;
	mTrans.translation(this->transform().translation());
	mTotal = mTrans * mMov;
	this->arrow->transform(mTotal);
}

void Pacman::eat() {
	std::pair<int, int> pos = this->getFieldPositionInFront();
	this->pField->removePoint(pos.first, pos.second);
}


void Pacman::teleport() {
	if (this->getFieldTypeInFront() == FieldType::Portal) {
		std::pair<int, int> portalPos = this->getFieldPositionInFront();
		std::pair<int, int> portalA(1, 15);
		std::pair<int, int> portalB(28, 15);
		if (portalPos == portalA) {
			this->setFieldPosition(portalB.first, this->transform().translation().Y, portalB.second, -M_PI / 2.0f);
		}
		else {
			this->setFieldPosition(portalA.first, this->transform().translation().Y, portalA.second, M_PI / 2.0f);
		}
	}
	return;
}


