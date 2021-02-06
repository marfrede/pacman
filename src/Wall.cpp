#include "Wall.h"

Wall::Wall(int planeWidth, int planeDepth, int width, float height, int depth, int posX, int posZ, BaseShader* pShader, float padding)
	: TriangleBoxModel(width - padding, height, depth - padding)
{
	this->shader(pShader, true);
	Matrix t;
	t.translation(
		((float)width) / 2.0f + ((float)posX) - ((float)planeWidth) / 2.0f,
		((float)height) / 2.0f,
		((float)depth) / 2.0f + ((float)posZ) - ((float)planeDepth) / 2.0f
	);
	this->transform(t);
}
