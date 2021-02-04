//
//  Pacman.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 01.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef Pacman_hpp
#define Pacman_hpp

#include "TriangleBoxModel.h"
#include "PhongShader.h"
#include "GameCharacter.hpp"
#include "EgoCam.hpp"
#include <stdio.h>

class Pacman : public GameCharacter
{
public:    
     
    Pacman(int planeWidth, int planeDepth, int posX, int posZ, PhongShader* pPhongShader);
    
    void update(float dtime);
    
    /**
     STEERING
     */
    /**
     * steer object
     * @param dtime deltatime
     */
    void steer(float dtime);
    
    //void setEgoCam(EgoCam cam){paccam=&cam;}
    //EgoCam* paccam;
    //void movePaccam();
    
private:
    
    
};

#endif /* Pacman_hpp */
