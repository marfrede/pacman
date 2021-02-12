//
//  Ghost.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 04.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Ghost.hpp"

Ghost::Ghost(int posX, int posZ, Color c) : GameCharacter(posX, 0.8f, posZ) {
    ConstantShader* pShader = new ConstantShader();
    pShader->color(c);
    this->shader(pShader, true);
}

Ghost::~Ghost() {
    //delete this->pShader; (BaseModel does it)
}

void Ghost::steer(float dtime) {
    
    if(!doCurrentAction(dtime)) {
        
        std::vector<int> actionList;
        bool added = false;
        
        if(checkFront()) {
            actionList.push_back(1);
            added = true;
        }
        if(checkLeft()) {
            actionList.push_back(2);
            added = true;
        }
        if(checkRight()) {
            actionList.push_back(3);
            added = true;
        }
        
        if(!added) {
            angleToTurn = 180;
        } else {
            int action = actionList.at((rand() % actionList.size()));
            //int action = actionList
            if(action == 1) {
                moveUnits = 1;
            } else if(action == 2) {
                angleToTurn = -90;
                moveUnits = 1;
            } else if(action == 3) {
                angleToTurn = 90;
                moveUnits = 1;
            } else {
                return;
            }
        }
        
        
        
        doCurrentAction(dtime);
        
    }
    
}
