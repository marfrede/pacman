#ifndef Field_hpp
#define Field_hpp

#ifdef WIN32
#define TEXTURE_DIRECTORY "../../assets/texture/"
#else
#define TEXTURE_DIRECTORY "../assets/texture/"
#endif

#include <list>
#include "constantshader.h"
#include "phongshader.h"
#include "basemodel.h"
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "Wall.h"

class Field
{
public:
	Field(int planeWidth, int planeDepth);
	typedef std::list<BaseModel*> ModelList;
	ModelList getWalls() { return this->Walls; }
	BaseModel* getPlane() { return this->pPlane; }
	void draw(const Camera camera);
	void end();
private:
	int planeWidth;
	int planeDepth;
	ConstantShader* pConstShader;
	PhongShader* pPhongShader;
	ModelList Walls;
	BaseModel* pPlane;
	void createField();
	void createWalls();
};
#endif /* Field_hpp */
