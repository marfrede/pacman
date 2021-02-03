#include "Wall.h"

Wall::Wall(int planeWidth, int planeDepth, int width, int height, int depth, int posX, int posZ, PhongShader* pPhongShader)
	: TriangleBoxModel(width - 0.5f, height, depth - 0.5f) // 0.5f padding
{
	this->shader(pPhongShader, true);
	Matrix t;
	t.translation(
		((float)width) / 2.0f + ((float)posX) - ((float)planeWidth) / 2.0f,
		((float)height) / 2.0f,
		((float)depth) / 2.0f + ((float)posZ) - ((float)planeDepth) / 2.0f
	);
	this->transform(t);
}