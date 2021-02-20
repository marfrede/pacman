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
    
    
    
    this->lifetime -= dtime;
    
    if(this->lifetime < 0) {
        this->alive = false;
    }
    
}

void Particle::draw(const BaseCamera& Cam)
{
    TrianglePlaneModel::draw(Cam);

    //Cam.getViewMatrix()
    Matrix m = this->transform();
    
    //Cam.g
    
    m.m00 = Cam.getViewMatrix().m00;
    m.m01 = Cam.getViewMatrix().m01;
    m.m02 = Cam.getViewMatrix().m02;
    m.m10 = Cam.getViewMatrix().m10;
    m.m11 = Cam.getViewMatrix().m11;
    m.m12 = Cam.getViewMatrix().m12;
    m.m20 = Cam.getViewMatrix().m20;
    m.m21 = Cam.getViewMatrix().m21;
    m.m22 = Cam.getViewMatrix().m22;
    
    //m.rotationZ(90);
    this->transform(m);
    /*
    Matrix mTotal, mTrans;
    mTrans.translation(this->velocity * 0.001f);
    mTotal = this->transform() * mTrans;
    this->transform(mTotal);
    */
    
    
    VB.activate();
    
    glDrawArrays(GL_LINES, 0, VB.vertexCount());
    
    VB.deactivate();
}

void Particle::reset(Vector pos) {
    Matrix reset;
    reset.identity();
    this->transform(reset);
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
