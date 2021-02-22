//
//  ParticleEmitter.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 21.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef ParticleEmitter_hpp
#define ParticleEmitter_hpp

#ifdef WIN32
#define TEXTURE_DIRECTORY "../../assets/texture/"
#else
#define TEXTURE_DIRECTORY "../assets/texture/"
#endif

#include <stdio.h>
#include <list>
#include "Particle.hpp"

class ParticleEmitter
{
public:
    ParticleEmitter(int maxParticles, Color color);
    ParticleEmitter(int maxParticles, BaseShader* pShader);
    ~ParticleEmitter();
    
    void setPosition(Vector position) {this->position = position;}
    void setColor(Color color) {this->color = color;}
    
    virtual void update(float dtime);
    virtual void draw(const Camera Cam);
    virtual void end();
    
protected:
    
    Vector position;
    Color color;
    
    int currentlyActiveBegin = 0;
    int currentlyActiveEnd = 0;
    
    std::list<Particle*> particles;
    
};

#endif /* ParticleEmitter_hpp */
