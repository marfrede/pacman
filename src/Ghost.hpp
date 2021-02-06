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
    
    Ghost(int planeWidth, int planeDepth, float posX, float posZ, PhongShader* pPhongShader);
    
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
