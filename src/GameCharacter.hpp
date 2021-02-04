//
//  GameCharacter.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 03.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef GameCharacter_hpp
#define GameCharacter_hpp
#define M_PI 3.141592653589793238462643

#include "TriangleBoxModel.h"
#include "PhongShader.h"
#include <stdio.h>
#include <list>

class GameCharacter : public TriangleBoxModel
{
public:
    typedef std::list<BaseModel*> WallList;
    /**
    * create a TriangleBoxModel at given position
    * @param planeWidth playing field width
    * @param planeDepth playing field depth
    * @param width in x-direction of the wall
    * @param height in y-direction of the wall
    * @param depth in z-direction of the wall
    * @param posX (min:0, max:planeWidth) x position on the planeModel
    * @param posZ (min:0, max:planeDepth) z position on the planeModel
    * @param pPhongShader to use for the walls
    */
    GameCharacter(int planeWidth, int planeDepth, int posX, int posZ, PhongShader* pPhongShader);
    
    void setWindow(GLFWwindow* window) {pWindow = window;}
    void setWalls(WallList walls) {Walls = walls;}
    
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
    
    
};

#endif /* GameCharacter_hpp */


