#include "Portal.h"
#ifndef M_PI
#define M_PI 3.1415926535897932
#endif // !M_PI


Portal::Portal(int posX, int posZ, Orientation orientation, BaseShader* pShader) :TrianglePlaneModel(1.0f, 1.0f, 1.0f, 1.0f)
{
	this->shader(pShader, false); // field deletes shader
	Matrix rotZ, t;
	rotZ.rotationZ((M_PI / 2.0f) * (orientation == Orientation::West ? -1 : 1));
	t.translation(
		(float)posX + (orientation == Orientation::East ? 0.99f : 0.01f) - (float)PLANE_WIDTH / 2.0f,
		0.6f,
		(float)posZ + 0.5f - (float)PLANE_DEPTH / 2.0f
	);
	this->transform(t * rotZ);
}

void Portal::update(float dtime) {
	Matrix rot;
	rot.rotationY(-dtime * 2);
	this->transform(this->transform() * rot);
}
