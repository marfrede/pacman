#include "Point.h"

Point::Point(int planeWidth, int planeDepth, int posX, int posZ, float radius) : TriangleSphereModel(radius) {
	this->initShader();
	this->init(planeWidth, planeDepth, posX, posZ);
}

Point::Point(int planeWidth, int planeDepth, int posX, int posZ, float radius, PhongShader* pPhongShader) : TriangleSphereModel(radius) {
	this->shader(pPhongShader, true);
	this->init(planeWidth, planeDepth, posX, posZ);
}

void Point::init(int planeWidth, int planeDepth, int posX, int posZ) {
	this->planeWidth = planeWidth;
	this->planeDepth = planeDepth;
	this->goingUp = true;
	float height = LO_HEIGHT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI_HEIGHT - LO_HEIGHT)));
	this->setPosition(posX, height, posZ);
}

void Point::setPosition(float x, float y, float z) {
	this->pos = Vector(x, y, z);
	Matrix t;
	t.translation(
		0.5f + (this->pos.X) - ((float)this->planeWidth) / 2.0f,
		this->pos.Y,
		0.5f + (this->pos.Z) - ((float)this->planeDepth) / 2.0f
	);
	this->transform(t);
}

void Point::initShader() {
	pPhongShader = new PhongShader();
	pPhongShader->ambientColor(Color(0.2f, 0.2f, 0.2f));
	pPhongShader->diffuseColor(Color(1.0f, 1.0f, 1.0f));
	pPhongShader->specularColor(Color(1.0f, 1.0f, 1.0f));
	this->shader(pPhongShader, true);
}

void Point::update(float dtime) {
	float newY = this->pos.Y;
	if (goingUp && this->pos.Y < HI_HEIGHT) {
		newY = this->pos.Y += dtime / 5.0f;
	}
	else if (!goingUp && this->pos.Y > LO_HEIGHT) {
		newY = this->pos.Y -= dtime / 5.0f;
	}
	else {
		this->goingUp = !this->goingUp;
	}
	this->setPosition(this->pos.X, newY, this->pos.Z);
}

Point::~Point() {
	delete this->pPhongShader;
}