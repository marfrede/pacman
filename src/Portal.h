
#ifndef Portal_h
#define Portal_h

#include "TrianglePlaneModel.h"
#include "BaseShader.h"
#include "Orientation.h"
#include "FieldSizeConstants.h"
#include "Lights.h"
#include "ShaderLightMapper.h"
#include "Pi.h"

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
	~Portal();
	void update(float dtime);
private:
	PointLight* pointLight;
};

#endif
