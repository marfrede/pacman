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
#include <stdio.h>

class Pacman : public GameCharacter
{
public:    
    
    Pacman(int planeWidth, int planeDepth, int posX, int posZ, PhongShader* pPhongShader);
    
private:
    
    
};

#endif /* Pacman_hpp */
