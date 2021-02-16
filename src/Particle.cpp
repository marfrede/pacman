//
//  Particle.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Particle.hpp"

Particle::Particle(Vector position, Vector velocity, float rotation, float lifetime, float gravity) : LinePlaneModel(1, 1, 1, 1) {

    this->transform.translation(position);
    this->transform.rotationZ(rotation);
    this->velocity = velocity;
    this->lifetime = lifetime;
    this->gravity = gravity;
    this->alive = true;
    
}

void Particle::update(float dtime) {
    
    this->position.X += velocity.X * dtime;
    this->position.Y += velocity.Y * gravity * dtime;
    //Z auslassen, da Kamerarichtung
    
    this->transform.translation(this->position);
    this->transform.rotationY(1); //Rotation in eine Richtung
    
    this->lifetime -= dtime;
    
    if(this->lifetime < 0) {
        this->alive = false;
    }
    
}

void Particle::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();
    
    glDrawArrays(GL_LINES, 0, VB.vertexCount());
    
    VB.deactivate();
}
