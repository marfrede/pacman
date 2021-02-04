//
//  EgoCam.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 04.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "EgoCam.hpp"

EgoCam::EgoCam(GLFWwindow* pWin, BaseModel* bm) : Camera(pWin)
{
    if(pWindow)
        glfwGetWindowSize(pWindow, &WindowWidth, &WindowHeight);
    
    this->obj = bm;
    
    m_ViewMatrix.identity();
    m_ProjMatrix.perspective((float)M_PI*65.0f/180.0f, (float)WindowWidth/(float)WindowHeight, 0.045f, 1000.0f);
}

void EgoCam::update() {
    
    Vector Pos = obj->transform().translation(); //m_Position + m_Panning + m_Zoom + m_Rotation;
    Vector Target = obj->transform().translation() + obj->transform().right(); //m_Target + m_Panning;
    m_ViewMatrix.lookAt(Target, m_Up, Pos);
    
}
