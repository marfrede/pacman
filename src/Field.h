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
#include "FieldType.h"

class Field
{
public:
	Field(int planeWidth, int planeDepth);
	typedef std::list<BaseModel*> ModelList;
	ModelList getWalls() { return this->Walls; }
	BaseModel* getPlane() { return this->pPlane; }

	/* get fieldtype (wall / point / free) in any field @param x | posX (0 - 29) @param z | posZ (0 - 32) */
	FieldType getFieldType(int x, int z);
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

	/* map origin position (x, z) to expansion (width, depth) */
	std::map<std::pair<int, int>, std::pair<int, int>> wallPositions;

	/* whole map as 2D Array */
	FieldType* fieldTypesMap = NULL;
	FieldType* getFieldTypesMap();
	void printFieldTypesMap();
};
#endif /* Field_hpp */
