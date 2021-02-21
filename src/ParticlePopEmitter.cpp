//
//  ParticlePopEmitter.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "ParticlePopEmitter.hpp"

ParticlePopEmitter::ParticlePopEmitter(int maxParticles, Color color) : ParticleEmitter(maxParticles, color) {
    
}

void ParticlePopEmitter::trigger(Vector pos, int quantity) {
    
    //std::cout << "**TRIGGER**" << std::endl;
    
    int tmpQuantity = quantity;
    
    //Exception wenn alle aktiv sind
    
    for(std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
        
        if((*it)->isAlive() == false && tmpQuantity > 0) {
            
            (*it)->reset(pos);
            
            tmpQuantity--;
            
        }
        
    }
    
}

void ParticlePopEmitter::update(float dtime) {

    //std::cout << "# Update Start #" << std::endl;
    
    for(std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
        
        //std::cout << counter << std::endl;
        
        if((*it)->isAlive() == true) {
            
            (*it)->update(dtime);
            
            //std::cout << "Er lebt!: " << (*it)->transform().translation() << std::endl;
            //(*it)->
        }
        
    }
    
   // std::cout << "# Update End #" << std::endl;
    
}

void ParticlePopEmitter::draw(const Camera Cam) {
        
        for(std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
            
            if((*it)->isAlive() == true) {
                
                (*it)->draw(Cam);
                
            }
            
        }
}
