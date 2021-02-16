
#ifndef Point_hpp
#define Point_hpp
#include "TriangleSphereModel.h"
#include "BaseShader.h"
#include "ShaderLightMapper.h"
#include "Lights.h"
#include "FieldSizeConstants.h"
#include "ParticlePopEmitter.hpp"

#define LO_HEIGHT 0.35f
#define HI_HEIGHT 0.65f

#define LIGHTING false

class Point : public TriangleSphereModel
{
public:
	Point(int posX, int posZ, float radius, BaseShader* pShader);
	void update(float dtime);
	~Point();
    ParticlePopEmitter ppe = NULL;
private:
	Vector pos;
	bool goingUp;
	void init(int posX, int posZ);
	void setPosition(float x, float y, float z);
	PointLight* pPointLight = NULL;
};

#endif /* Point_hpp */
