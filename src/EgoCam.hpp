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

#ifndef M_PI
#define M_PI 3.1415926535897932
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
