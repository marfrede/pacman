//
//  Particle.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "vector.h"
#include "matrix.h"
#include "LinePlaneModel.h"

class Particle : public LinePlaneModel
{
public:
    Particle(Vector position, Vector velocity, float rotation, float lifetime, float gravity);
    ~Particle() {};
    
    void setPosition(Vector position) {this->position = position;}
    void setAlive(bool alive) {this->alive = alive;}
    bool isAlive() {return this->alive;}
    
    void update(float dtime);
    void draw(const BaseCamera& Cam);
    
protected:
    Matrix transform;
    
    Vector position;
    Vector velocity;
    float rotation;
    float lifetime;
    float gravity;
    
    bool alive;
};

#endif /* Particle_hpp */
