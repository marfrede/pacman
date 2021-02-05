
#ifndef Point_hpp
#define Point_hpp
#include "TriangleSphereModel.h"
#include "PhongShader.h"
#define LO_HEIGHT 0.35f
#define HI_HEIGHT 0.65f

class Point : public TriangleSphereModel
{
public:
	Point(int planeWidth, int planeDepth, int posX, int posZ, float radius);
	Point(int planeWidth, int planeDepth, int posX, int posZ, float radius, PhongShader* pPhongShader);
	void update(float dtime);
	~Point();
private:
	PhongShader* pPhongShader;
	Vector pos;
	bool goingUp;
	void initShader();
	void init(int planeWidth, int planeDepth, int posX, int posZ);
	void setPosition(float x, float y, float z);
	int planeWidth;
	int planeDepth;
};

#endif /* Point_hpp */
