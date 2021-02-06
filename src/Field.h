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
#include "Point.h"
#include "FieldType.h"

class Field
{
public:
	Field(int planeWidth, int planeDepth);
	typedef std::list<BaseModel*> ModelList;
	typedef std::list<Point*> PointList;
	ModelList getWalls() { return this->Walls; }

	/* get fieldtype (wall / point / free) in any field @param x | posX (0 - 29) @param z | posZ (0 - 32) */
	FieldType getFieldType(int x, int z);
	void draw(const Camera camera);
	void update(float dtime);
	void end();
private:
	int planeWidth;
	int planeDepth;
	ConstantShader* pShaderPlane;
	PhongShader* pShaderWall;
	ConstantShader* pShaderPoint;

	BaseModel* pPlane;
	ModelList Walls;
	PointList Points;
	void createField();
	void initWallPositions();
	void initFieldTypesMap();
	void createWalls();
	void createPoints();

	/* map origin position (x, z) to expansion (width, depth) */
	std::map<std::pair<int, int>, std::pair<int, int>> wallPositions;
	/* whole map as 2D Array */
	FieldType* fieldTypesMap = NULL;
	void printFieldTypesMap();
};
#endif /* Field_hpp */
