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
#define SHOW_PLANE true
#define SHOW_WALLS true
#define SHOW_POINTS true
#define SHOW_PORTALS true

#define WALL_HEIGHT 5.5f
#define WALL_PADDING 0.0f

#define POINT_RADIUS 0.05f

class Field
{
public:
	Field(float wallHeight);
	~Field();
	typedef std::list<BaseModel*> ModelList;
	typedef std::list<Portal*> PortalList;
	typedef std::map<std::pair<int, int>, Point*> PointList;

	void reset();

	/* convert global position to field position */
	std::pair<int, int> getPosOnField(Vector position);

	/* get fieldtype (wall / point / free) in any field @param x | posX (0 - 29) @param z | posZ (0 - 32) */
	FieldType getFieldType(int x, int z);

	/* get fieldtype (wall / point / free) in from any position @param Vector(float, float, float) */
	FieldType getFieldType(Vector position);

	/**
	* removes the point on the given position and delete point object
	* @return true | when pacman has eaten the point, false | when there was no point at all on this field
	*/
	bool removePoint(int posX, int posZ);

	/* get global position of the closest Point point from global position origin on the field */
	Vector closestPointPos(Vector origin);

	/**
	* returns false if all points are consumed
	* returns true if points are left OR no points are generated at all (e.g. Debug Mode)
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
	void createWalls(float wallHeight);
	void createPoints();
	void createPortals();
	void initWallPositions();
	void initFieldTypesMap();

	/* map origin position (x, z) to expansion (width, depth) */
	std::map<std::pair<int, int>, std::pair<int, int>> wallPositions;

	/* whole map as 2D Array */
	FieldType* fieldTypesMap = NULL;
    
    /* WallHeight */
    float wallHeight = 5.5f;

	void printFieldTypesMap();
};
#endif /* Field_hpp */
