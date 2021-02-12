//
//  GameCharacter.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 03.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef GameCharacter_hpp
#define GameCharacter_hpp

#include "TriangleBoxModel.h"
#include "PhongShader.h"
#include "FieldSizeConstants.h"
#include "FieldType.h"
#include "Orientation.h"
#include "Field.h"
#include <stdio.h>
#include <list>

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif

class GameCharacter : public TriangleBoxModel
{
    
public:

	typedef std::list<BaseModel*> WallList;

	/**
	* create a GameCharacter at given position
	* @param posX (min:0, max:planeWidth) x position on the planeModel
	* @param y (min:-∞, max:∞) models y position
	* @param posZ (min:0, max:planeDepth) z position on the planeModel
	* @param pShader to use for the character
	*/
	GameCharacter(int posX, float y, int posZ);
	~GameCharacter();
	void setWindow(GLFWwindow* window) { pWindow = window; }
	void setField(Field* field) { pField = field; }
	void setPointLight(PointLight* pL) { pointLight = pL; }
	void setSpotLight(SpotLight* sL) { spotLight = sL; }

	virtual void update(float dtime);

	/**
	 STEERING
	 */
	 /**
	  * steer object
	  * @param dtime deltatime
	  */
	virtual void steer(float dtime) {/* insert code here */ };
	/**
	 * rotate object
	 * @param dtime deltatime
	 * @param left direction
	 */
	void rotate(float dtime, bool left);
	/**
	 * move object
	 * @param dtime deltatime
	 */
	void move(float dtime);
	/**
	 * perform current action
	 * @param dtime deltatime
	 */
	bool doCurrentAction(float dtime);

	// WALL CHECK
	/**
	* check the front field
	* @returns true if the character can make a step forward (Point or Free ahead) | false if the character can not make a step forward (Wall ahead)
	*/
	bool checkFront();
    /**
    * check the left field
    * @returns true if the character can make a step to the left (Point or Free ahead) | false if the character can not make a step left (Wall ahead)
    */
    bool checkLeft();
    /**
    * check the left field
    * @returns true if the character can make a step to the right (Point or Free ahead) | false if the character can not make a step right (Wall ahead)
    */
    bool checkRight();

	//Sub Movement
    virtual void moveSubs();

	// FIELD POSITION INFOS
	 /** get the field the character stands on as first = posX, second = posZ (0, 0 would be top left corner) */
	std::pair<int, int> getFieldPosition();
	/** get the field type the character stands on as enum {Wall, Point, Free} */
	FieldType getFieldType();
	/** get the field type the character is looking at as enum {Wall, Point, Free} */
	FieldType getFieldTypeInFront();
    /** get the field type left where the character is looking at as enum {Wall, Point, Free} */
    FieldType getFieldTypeToLeft();
    /** get the field type right where the character is looking at as enum {Wall, Point, Free} */
    FieldType getFieldTypeToRight();
	/** get the orientation the character is looking in as enum {North (up), East (right), South (down), West (left)} */
	Orientation getOrientation();


protected:

	//Spielfeld
	Field* pField;

	GLFWwindow* pWindow;

	//STEERING
	//Rotation
	float angleToTurn;
	float rotateSpeed = 200;
	//Moving
	float moveUnits;
	float movingSpeed = 3;
	//Lighting
	PointLight* pointLight = NULL;
	SpotLight* spotLight = NULL;
};

#endif /* GameCharacter_hpp */


