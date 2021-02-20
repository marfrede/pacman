//
//  ParticleEmitter.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef ParticleEmitter_hpp
#define ParticleEmitter_hpp

#include <stdio.h>
#include <list>
#include "Particle.hpp"

class ParticleEmitter
{
public:
    ParticleEmitter();
    
    void setPosition(Vector position) {this->position = position;}
    
    virtual void update(float dtime);
    virtual void draw(const Camera Cam);
    
protected:
    
    Vector position;
    int maxParticles = 100;
    
    int currentlyActiveBegin = 0;
    int currentlyActiveEnd = 0;
    
    std::list<Particle*> particles;
    
};

#endif /* ParticleEmitter_hpp */
