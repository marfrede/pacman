#include "Point.h"

Point::Point(int planeWidth, int planeDepth, int posX, int posZ, float radius, PhongShader* pPhongShader) : TriangleSphereModel(radius) {
	this->shader(pPhongShader, true);
	this->init(planeWidth, planeDepth, posX, posZ);
}

void Point::init(int planeWidth, int planeDepth, int posX, int posZ) {
	this->planeWidth = planeWidth;
	this->planeDepth = planeDepth;
	this->goingUp = true;
	float height = LO_HEIGHT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI_HEIGHT - LO_HEIGHT)));
	this->pPointLight = new PointLight();
	this->pPointLight->color(Color(255.0f / 255.0f, 184.0f / 255.0f, 174.0f / 255.0f));
	this->pPointLight->attenuation(Vector(1.0f, 1.0f, 1.0f));
	ShaderLightMapper::instance().addLight(this->pPointLight);
	this->setPosition(posX, height, posZ);
}

/* sets position on field */
void Point::setPosition(float x, float y, float z) {
	this->pos = Vector(x, y, z);
	Matrix t;
	t.translation(
		0.5f + (this->pos.X) - ((float)this->planeWidth) / 2.0f,
		this->pos.Y,
		0.5f + (this->pos.Z) - ((float)this->planeDepth) / 2.0f
	);
	this->transform(t);
	this->pPointLight->position(this->transform().translation());
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
	delete this->pPointLight;
}