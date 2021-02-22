//
//  GameCharacter.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 03.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "GameCharacter.hpp"

GameCharacter::GameCharacter(int posX, float y, int posZ, const char* ModelFile, bool FitSize) : Model(ModelFile, FitSize) {
	this->setSpawnLocation(posX, posZ);
	this->setFieldPosition(posX, y, posZ);
	angleToTurn = 0.0f;
}

GameCharacter::GameCharacter(int posX, float y, int posZ) : Model() {
	this->setSpawnLocation(posX, posZ);
	this->setFieldPosition(posX, y, posZ);
	angleToTurn = 0.0f;
}

GameCharacter::~GameCharacter() {
	delete this->spotLight;
	delete this->pointLight;
	delete this->ext;
}

void GameCharacter::setPointLight(Vector attenuation, Color color) {
	this->pointLight = new PointLight();
	this->pointLight->color(color);
	this->pointLight->attenuation(attenuation);
	ShaderLightMapper::instance().addLight(this->pointLight);
	this->pointLight->position(this->transform().translation());
}
void GameCharacter::setSpotLight(float outerRadius, float innerRadius, Color color) {
	this->spotLight = new SpotLight();
	this->spotLight->color(color);
	this->spotLight->direction(Vector(0, -1, 0));
	this->spotLight->innerRadius(innerRadius);
	this->spotLight->outerRadius(outerRadius);
	ShaderLightMapper::instance().addLight(this->spotLight);
	this->spotLight->position(this->transform().translation());
}
void GameCharacter::setExt(const char* modelDir, Color color) {
	this->ext = new Model(modelDir);
	ConstantShader* cShader = new ConstantShader();
	cShader->color(color);
	ext->shader(cShader, true);
}

void GameCharacter::update(float dtime) {

	this->steer(dtime);
	this->moveSubs();
}

void GameCharacter::draw(const Camera Cam) {

	Model::draw(Cam);

	if (ext) {
		this->ext->draw(Cam);
	}

}

void GameCharacter::setSpawnLocation(int x, int y) {
	this->spawnLocation.first = x;
	this->spawnLocation.second = y;
}

bool GameCharacter::doCurrentAction(float dtime) {
	//Check rotation
	if (angleToTurn > 0) {
		//Führe Rotation nach rechts aus

		rotate(dtime, false);

		return true;

	}
	else if (angleToTurn < 0) {
		//Führe Rotation nach links aus

		rotate(dtime, true);

		return true;

	}
	else if (moveUnits > 0) { //Check movement
		if (moveUnits == 1) { // before entering the field
			this->eat();
			this->teleport();
		}
		move(dtime);

		return true;

	}
	else {
		return false;
	}
}


void GameCharacter::rotate(float dtime, bool left) {

	float rotateAngle = rotateSpeed * dtime;
	float angle = rotateAngle * (2 * M_PI) / 360;

	if (left) {

		angle *= -1;

		if (angleToTurn - rotateAngle > 0) {
			angle = angleToTurn * (2 * M_PI / 360);
			rotateAngle = angleToTurn;
		}

	}
	else {

		if (angleToTurn - rotateAngle < 0) {
			angle = angleToTurn * (2 * M_PI / 360);
			rotateAngle = angleToTurn;
		}

	}

	Matrix mTotal, mRot;
	mRot.rotationY(-angle);

	mTotal = this->transform() * mRot;
	this->transform(mTotal);

	if (left) {
		angleToTurn += rotateAngle;
	}
	else {
		angleToTurn -= rotateAngle;
	}

}

void GameCharacter::move(float dtime) {

	float movingUnits = movingSpeed * dtime;

	if ((moveUnits - movingUnits) < 0.0f) {
		movingUnits = moveUnits;
	}

	Matrix mTotal, mMov;
	mMov.translation(0, 0, movingUnits);

	mTotal = this->transform() * mMov;
	this->transform(mTotal);

	moveUnits -= movingUnits;

}

bool GameCharacter::checkFront() {
	return (this->getFieldTypeInFront() != FieldType::Wall);
}

bool GameCharacter::checkLeft() {
	return (this->getFieldTypeToLeft() != FieldType::Wall);
}

bool GameCharacter::checkRight() {
	return (this->getFieldTypeToRight() != FieldType::Wall);
}

std::pair<int, int> GameCharacter::getFieldPosition() {
	Vector pos = this->transform().translation();
	return std::pair<int, int>(
		(int)(pos.X + (float)PLANE_WIDTH / 2.0f),
		(int)(pos.Z + (float)PLANE_DEPTH / 2.0f)
		);
}

void GameCharacter::setFieldPosition(int posX, float y, int posZ) {
	Matrix t;
	float x = 0.5f + (float)posX - ((float)PLANE_WIDTH) / 2.0f;
	float z = 0.5f + (float)posZ - ((float)PLANE_DEPTH) / 2.0f;
	t.translation(x, y, z);
	this->transform(t);
	return;
}

void GameCharacter::setFieldPosition(int posX, float y, int posZ, float rotation) {
	Matrix t, rot;
	float x = 0.5f + (float)posX - ((float)PLANE_WIDTH) / 2.0f;
	float z = 0.5f + (float)posZ - ((float)PLANE_DEPTH) / 2.0f;
	t.translation(x, y, z);
	rot.rotationY(rotation);
	this->transform(t * rot);
	return;
}

std::pair<int, int> GameCharacter::getFieldPositionInFront() {
	std::pair<int, int> posOnField = this->getFieldPosition();
	switch (this->getOrientation())
	{
	case Orientation::North:
		posOnField.second--;
		break;
	case Orientation::East:
		posOnField.first++;
		break;
	case Orientation::West:
		posOnField.first--;
		break;
	default:
		posOnField.second++;
		break;
	}
	return posOnField;
}

FieldType GameCharacter::getFieldType() {
	std::pair<int, int> posOnField = this->getFieldPosition();
	return this->pField->getFieldType(
		posOnField.first,
		posOnField.second
	);
}

FieldType GameCharacter::getFieldTypeInFront() {
	std::pair<int, int> posInFrontOnField = this->getFieldPositionInFront();
	return this->pField->getFieldType(
		posInFrontOnField.first,
		posInFrontOnField.second
	);
}

FieldType GameCharacter::getFieldTypeToLeft() {
	std::pair<int, int> posOnField = this->getFieldPosition();
	switch (this->getOrientation())
	{
	case Orientation::North:
		posOnField.first--;
		break;
	case Orientation::East:
		posOnField.second--;
		break;
	case Orientation::West:
		posOnField.second++;
		break;
	default:
		posOnField.first++;
		break;
	}
	return this->pField->getFieldType(
		posOnField.first,
		posOnField.second
	);
}

FieldType GameCharacter::getFieldTypeToRight() {
	std::pair<int, int> posOnField = this->getFieldPosition();
	switch (this->getOrientation())
	{
	case Orientation::North:
		posOnField.first++;
		break;
	case Orientation::East:
		posOnField.second++;
		break;
	case Orientation::West:
		posOnField.second--;
		break;
	default:
		posOnField.first--;
		break;
	}
	return this->pField->getFieldType(
		posOnField.first,
		posOnField.second
	);
}

void GameCharacter::reset() {
	Matrix t;
	float x = 0.5f + (float)spawnLocation.first - ((float)PLANE_WIDTH) / 2.0f;
	float z = 0.5f + (float)spawnLocation.second - ((float)PLANE_DEPTH) / 2.0f;

	t.translation(x, 0.8f, z);
	this->transform(t);

	this->angleToTurn = 0.0f;
	this->moveUnits = 0;
}

void GameCharacter::moveSubs() {

	if (pointLight) {
		// std::cout << "Hat Punktlight" << std::endl;
		this->pointLight->position(this->transform().translation());
		// std::cout << this->pointLight->position() << std::endl;
	}
	if (spotLight) {
		// std::cout << "Hat Spotlight" << std::endl;
		this->spotLight->position(this->transform().translation());
		// std::cout << this->spotLight->position() << std::endl;
	}
	if (ext) {
		this->ext->transform(this->transform());
	}

}

Orientation GameCharacter::getOrientation() {
	Vector orientation = this->transform().forward();
	if (orientation.Z > 0.5f) {
		return Orientation::South;
	}
	else if (orientation.Z < -0.5f) {
		return Orientation::North;
	}
	else if (orientation.X > 0.5f) {
		return Orientation::East;
	}
	else {
		return Orientation::West;
	}
}
