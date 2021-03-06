
#ifndef Wall_h
#define Wall_h

#include "TriangleBoxModel.h"
#include "BaseShader.h"
#include "FieldSizeConstants.h"

class Wall : public TriangleBoxModel
{
public:
	/**
	* create a TriangleBoxModel at given position
	* @param width in x-direction of the wall
	* @param height in y-direction of the wall
	* @param depth in z-direction of the wall
	* @param posX (min:0, max:planeWidth) x position on the planeModel
	* @param posZ (min:0, max:planeDepth) z position on the planeModel
	* @param pShader to use for the walls
	*/
	Wall(int width, float height, int depth, int posX, int posZ, BaseShader* pShader, float padding);
};

#endif
