//
//  Ghost.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 04.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Ghost.hpp"

Ghost::Ghost(int posX, int posZ, PhongShader* pPhongShader) : GameCharacter(posX, 0.8f, posZ, pPhongShader) {
    
}

void Ghost::steer(float dtime) {
    
    if(!doCurrentAction(dtime)) {
        
        std::vector<int> actionList;
        
        //Check if front free
        actionList.push_back(1);
        //Check if left free
        actionList.push_back(2);
        //Check if right free
        actionList.push_back(3);
        
        int action = actionList.at((rand() % actionList.size()));
        //int action = actionList
        if(action == 1) {
            moveUnits = 1;
        } else if(action == 2) {
            angleToTurn = 90;
            moveUnits = 1;
        } else if(action == 3) {
            angleToTurn = -90;
            moveUnits = 1;
        } else {
            return;
        }
        
        doCurrentAction(dtime);
        
    }
    
}
