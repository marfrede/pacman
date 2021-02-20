//
//  Particle.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Particle.hpp"

Particle::Particle(Vector position, Vector velocity, float rotation, float lifetime, float gravity) : TrianglePlaneModel(1, 1, 1, 1) {

    Matrix mTotal, mTrans;
    mTotal = this->transform() * mTrans.translation(position);
    this->transform(mTotal);
    //this->transform().rotationZ(rotation);
    
}

void Particle::update(float dtime) {
    
    //std::cout << "Update Particle" << std::endl;
    
    Matrix mTotal, mTrans;
    mTrans.translation(this->velocity * 0.001f);
    mTotal = this->transform() * mTrans;
    this->transform(mTotal);
    
    this->lifetime -= dtime;
    
    if(this->lifetime < 0) {
        this->alive = false;
    }
    
}

void Particle::draw(const BaseCamera& Cam)
{
    TrianglePlaneModel::draw(Cam);

    VB.activate();
    
    glDrawArrays(GL_LINES, 0, VB.vertexCount());
    
    VB.deactivate();
}

void Particle::reset(Vector pos) {
    
    Matrix mTrans, mTotal;
    mTrans.translation((pos - this->transform().translation()));
    mTotal = this->transform() * mTrans;
    this->transform(mTotal);
    
    this->alive = true;
    this->lifetime = 300;//rand() % 50);
    Vector velo = Vector((rand() % 10), (rand() % 10), (rand() % 10));
    this->velocity = velo.normalize();
    
    this->rotation = 1;
    
}
