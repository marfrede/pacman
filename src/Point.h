
#ifndef Point_hpp
#define Point_hpp
#include "TriangleSphereModel.h"
#include "BaseShader.h"
#include "ShaderLightMapper.h"
#include "Lights.h"
#define LO_HEIGHT 0.35f
#define HI_HEIGHT 0.65f

class Point : public TriangleSphereModel
{
public:
	Point(int planeWidth, int planeDepth, int posX, int posZ, float radius, BaseShader* pShader);
	void update(float dtime);
	~Point();
private:
	Vector pos;
	bool goingUp;
	void init(int planeWidth, int planeDepth, int posX, int posZ);
	void setPosition(float x, float y, float z);
	int planeWidth;
	int planeDepth;
	PointLight* pPointLight;
};

#endif /* Point_hpp */
