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
    void setWindow(GLFWwindow* window) {pWindow = window;}
    void setWalls(WallList walls) {Walls = walls;}
    void setPointLight(PointLight* pL) {pointLight = pL;}
    void setSpotLight(SpotLight* sL) {spotLight = sL;}
    
    virtual void update(float dtime);
    
    /**
     STEERING
     */
    /**
     * steer object
     * @param dtime deltatime
     */
    virtual void steer(float dtime) {/* insert code here */};
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
    
    
    /**
     WALL CHECK
     */
    bool checkFront();
    bool checkLeft();
    bool checkRight();
    
    //Lighting
    void moveLights();
    
protected:
    
    //Spielfeld
    WallList Walls;
    
    GLFWwindow* pWindow;
    
    //STEERING
    //Rotation
    float angleToTurn;
    float rotateSpeed = 200;
    //Moving
    float moveUnits;
    float movingSpeed = 3;
    //Lighting
    PointLight* pointLight;
    SpotLight* spotLight;
};

#endif /* GameCharacter_hpp */


