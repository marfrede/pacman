//
//  EgoCam.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 04.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef EgoCam_hpp
#define EgoCam_hpp

#include "Camera.h"
#include "BaseModel.h"
#include <stdio.h>

#ifdef WIN32
#define M_PI 3.141592653589793238462643;
#endif

class EgoCam : public Camera
{
public:
    EgoCam(GLFWwindow* pWin, BaseModel* bm);
    
    void update();
    
    void setObj(BaseModel* bm) {obj = bm;}
    
private:
    
    BaseModel* obj;
    
};

#endif /* EgoCam_hpp */
