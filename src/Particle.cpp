//
//  Particle.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Particle.hpp"

Particle::Particle(Vector position, Vector velocity, float rotation, float lifetime, float gravity) : TrianglePlaneModel(0.1f, 0.1f, 1, 1) {

    Matrix mTotal, mTrans, mRot;
    mRot.rotationY(M_PI/2);
    mTrans.translation(position);
    mTotal = this->transform() * mTrans * mRot;
    
    this->transform(mTotal);
    //this->transform().rotationZ(rotation);
    
}

void Particle::update(float dtime) {
    
    //std::cout << "Update Particle" << std::endl;
    
    this->position = this->position + (this->velocity * 0.0001f);
    
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
    
    /* Invertiere right */
    m.m00 = Cam.getViewMatrix().m00;
    m.m01 = Cam.getViewMatrix().m10;
    m.m02 = Cam.getViewMatrix().m20;
    /* Invertiere up */
    m.m10 = Cam.getViewMatrix().m01;
    m.m11 = Cam.getViewMatrix().m11;
    m.m12 = Cam.getViewMatrix().m21;
    /* Invertiere forward */
    m.m20 = Cam.getViewMatrix().m02;
    m.m21 = Cam.getViewMatrix().m12;
    m.m22 = Cam.getViewMatrix().m22;
    
    this->transform(m);
    
    /* Da Plane liegt, muss dies mit mROt ausgeglichen werden und mTrans erst nach initialer Cameraviewmatrixrotation */
    Matrix mTotal, mRot, mTrans, mRotZ;
    mRot.rotationX(M_PI/2);
    mRotZ.rotationZ(this->rotation);
    mTrans.translation(this->position - this->transform().translation());
    mTotal = this->transform() * mTrans * mRotZ * mRot;
    this->transform(mTotal);
    
    VB.activate();
    
    glDrawArrays(GL_LINES, 0, VB.vertexCount());
    
    VB.deactivate();
}

void Particle::reset(Vector pos) {
    this->position = pos;
    Matrix reset;
    reset.identity();
    this->transform(reset);
    Matrix mTotal, mRot, mTrans, mRotZ;
    mRot.rotationX(M_PI/2);
    mRotZ.rotationZ(this->rotation);
    mTrans.translation(this->position - this->transform().translation());
    mTotal = this->transform() * mTrans * mRotZ * mRot;
    this->transform(mTotal);
    this->alive = true;
    this->lifetime = (rand() % 50);
    Vector velo = Vector((rand() % 10), (rand() % 10), (rand() % 10));
    this->velocity = velo.normalize();
    this->rotation = 1;
    
}
