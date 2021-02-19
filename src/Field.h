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
#include "Portal.h"
#include "Point.h"
#include "FieldType.h"
#include "FieldSizeConstants.h"

#define SHOW_DEBUG_PLANE false
#define SHOW_PLANE false
#define SHOW_WALLS true
#define SHOW_POINTS true
#define SHOW_PORTALS true

#define WALL_HEIGHT 0.5f
#define WALL_PADDING 0.0f

#define POINT_RADIUS 0.05f

class Field
{
public:
	Field();
	~Field();
	typedef std::list<BaseModel*> ModelList;
	typedef std::list<Portal*> PortalList;
	typedef std::map<std::pair<int, int>, Point*> PointList;

	void reset();

	/* get fieldtype (wall / point / free) in any field @param x | posX (0 - 29) @param z | posZ (0 - 32) */
	FieldType getFieldType(int x, int z);

	/**
	* removes the point on the given position
	* @return true | when pacman has eaten the point, false | when there was no point at all on this field
	*/
	Vector closestPointPos(Vector origin);
	bool removePoint(int posX, int posZ);
	/**
	* returns false if all points are consumed 
	* returns true if points are left OR no points are generated at all
	*/
	bool pointsLeft();

	void draw(const Camera camera);
	void update(float dtime);
	void end();
private:
	PhongShader* pShaderWall;
	PhongShader* pShaderPortal;
	ConstantShader* pShaderPoint;

	BaseModel* pPlane;
	LinePlaneModel* pPlaneDebug;
	ModelList Walls;
	PortalList Portals;
	PointList Points;
	void createField();
	void initWallPositions();
	void initFieldTypesMap();
	void createWalls();
	void createPoints();
	void createPortals();

	/* map origin position (x, z) to expansion (width, depth) */
	std::map<std::pair<int, int>, std::pair<int, int>> wallPositions;
	/* whole map as 2D Array */
	FieldType* fieldTypesMap = NULL;
	void printFieldTypesMap();
};
#endif /* Field_hpp */
