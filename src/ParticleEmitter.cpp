//
//  ParticleEmitter.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "ParticleEmitter.hpp"
#include <list>
#include "ConstantShader.h"

ParticleEmitter::ParticleEmitter(int quantity) {
    
    for(int i=0; i<quantity; i++) {
        Particle* p = new Particle(this->position,
                                               Vector(0,1,0),
                                               45,
                                               200,
                                  1);
        ConstantShader* cShader = new ConstantShader();
        cShader->color(Color(1,1,1));
        p->shader(cShader);
        this->particles.push_back(p);
    }
    
}

void ParticleEmitter::update(float dtime) {
    
    for(std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
        
        (*it)->update(dtime);
        
        if((*it)->isAlive() == false) {
            (*it)->setPosition(this->position);
            (*it)->setAlive(true);
        }
        
    }
    
}

void ParticleEmitter::draw(const Camera Cam) {
    for(std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
        
        (*it)->draw(Cam);
        
    }
}
