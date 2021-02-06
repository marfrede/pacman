//
//  Ghost.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 04.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef Ghost_hpp
#define Ghost_hpp

#include <stdio.h>
#include "GameCharacter.hpp"

class Ghost : public GameCharacter
{
public:

    /**
    * create a Ghost at given position
    * @param posX (min:0, max:planeWidth) x position on the field
    * @param posZ (min:0, max:planeDepth) z position on the field
    * @param pPhongShader to use for the ghost
    */
    Ghost(int posX, int posZ, PhongShader* pPhongShader);
    
    /**
     STEERING
     */
    /**
     * steer object
     * @param dtime deltatime
     */
    void steer(float dtime);
    
protected:
private:

};

#endif /* Ghost_hpp */
