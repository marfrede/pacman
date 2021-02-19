//
//  ConstantShader.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "FreetypeShader.hpp"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

FreetypeShader::FreetypeShader() : Col(1.0f,0.0f,0.0f)
{
    //ShaderProgram = createShaderProgram(VertexShaderCode, FragmentShaderCode);
    
    bool loaded = load(ASSET_DIRECTORY"vstext.glsl", ASSET_DIRECTORY"fstext.glsl");
    if (!loaded)
        throw std::exception();
    
    ColorLoc = glGetUniformLocation(ShaderProgram, "Color");
    assert(ColorLoc>=0);
    
}
void FreetypeShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
    
    glUniform3f(ColorLoc, Col.R, Col.G, Col.B);
}
void FreetypeShader::color( const Color& c)
{
    Col = c;
}

