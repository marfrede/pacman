//
//  ParticlePopEmitter.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "ParticlePopEmitter.hpp"

ParticlePopEmitter::ParticlePopEmitter(int quantity) : ParticleEmitter(quantity) {
    
}

void ParticlePopEmitter::trigger() {
    this->triggered = true;
    
    std::cout << "TRIGGERED!" << std::endl;
    
    std::cout << this->triggered << std::endl;
    std::cout << (this->triggered == true) << std::endl;
}

void ParticlePopEmitter::update(float dtime) {
    
    if(this->triggered == true) {
        
        if(this->particles.size() == 0) {
            this->empty = true;
        }
        
        std::cout << "im trigger" << std::endl;
        
        int counter = 0;
        
        for(std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
            
            std::cout << "Partikel " << counter << " !" << std::endl;
            
            (*it)->update(dtime);
            
            if((*it)->isAlive() == false) {
                (*it)->~Particle();
                it = particles.erase(it);
            }
            counter++;
            
        }
        
    }
    
}

void ParticlePopEmitter::draw(const Camera Cam) {
    if(this->triggered == true) {
        
        for(std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
            
            (*it)->draw(Cam);
            
        }
    }
}
