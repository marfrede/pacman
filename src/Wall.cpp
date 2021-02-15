#include "Wall.h"

Wall::Wall(int width, float height, int depth, int posX, int posZ, BaseShader* pShader, float padding)
	: TriangleBoxModel(width - padding, height, depth - padding)
{
    //std::cout << "NEW WALL" << std::endl;
	this->shader(pShader, false); // field deletes shader
	Matrix t;
	t.translation(
		((float)width) / 2.0f + ((float)posX) - ((float)PLANE_WIDTH) / 2.0f,
		((float)height) / 2.0f,
		((float)depth) / 2.0f + ((float)posZ) - ((float)PLANE_DEPTH) / 2.0f
	);
	this->transform(t);
}
