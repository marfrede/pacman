
#ifndef Portal_h
#define Portal_h

#include "TrianglePlaneModel.h"
#include "BaseShader.h"
#include "Orientation.h"
#include "FieldSizeConstants.h"

class Portal : public TrianglePlaneModel
{
public:
	/**
	* create a Portal at given position
	* @param posX (min:0, max:planeWidth) x position on the planeModel
	* @param posZ (min:0, max:planeDepth) z position on the planeModel
	* @param orientation (direction in which the portal points)
	*/
	Portal(int posX, int posZ, Orientation orientation, BaseShader* pShader);
	void update(float dtime);
};

#endif
