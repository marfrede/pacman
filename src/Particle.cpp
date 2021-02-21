//
//  Particle.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "Particle.hpp"

Particle::Particle(Vector position, Vector velocity, float rotation, float lifetime, float gravity) : TrianglePlaneModel(0.005f, 0.005f, 1, 1) {

    Matrix mTotal, mTrans, mRot;
    mRot.rotationY(M_PI/2);
    mTrans.translation(position);
    mTotal = this->transform() * mTrans * mRot;
    
    this->transform(mTotal);
    
}

void Particle::update(float dtime) {

    this->position = this->position + (this->velocity * dtime * 0.5f);
    this->lifetime -= dtime;
    this->rotation += this->rotation * dtime * 0.01f;
    
    Matrix mTotal, mRot, mTrans, mRotZ;
    mTotal.identity();
    
    mTrans = mTrans.translation(this->position);
    mTotal = mTotal * mTrans;
    this->transform(mTotal);
    
    if(this->lifetime < 0) {
        this->alive = false;
    }
    
}

void Particle::draw(const BaseCamera& Cam)
{
    Matrix m = this->transform();
    
    Matrix vm = Cam.getViewMatrix();
    /* Invertiere right */
    m.m00 = vm.m00;
    m.m01 = vm.m10;
    m.m02 = vm.m20;
    /* Invertiere up */
    m.m10 = vm.m01;
    m.m11 = vm.m11;
    m.m12 = vm.m21;
    /* Invertiere forward */
    m.m20 = vm.m02;
    m.m21 = vm.m12;
    m.m22 = vm.m22;
    
    this->transform(m);
    
    /* Da Plane liegt, muss dies mit mRot ausgeglichen werden */
    Matrix mTotal, mRot, mRot2;
    mRot.identity();
    mRot2.identity();
    mRot.rotationX(M_PI/2);
    mRot2.rotationZ(this->rotation * 180/M_PI);
    mTotal = this->transform() * mRot2 * mRot;
    this->transform(mTotal);
    
    TrianglePlaneModel::draw(Cam);
    
    VB.activate();
    
    glDrawArrays(GL_LINES, 0, VB.vertexCount());
    
    VB.deactivate();
}

void Particle::reset(Vector pos) {
    this->position = pos;
    this->alive = true;
    this->lifetime = ((rand() % 2)+1);

    Vector velo = Vector((rand() % 10 - 5), (rand() % 10 - 5), (rand() % 10 - 5));
    this->velocity = velo.normalize();
    this->rotation = ((rand() % 10) - 5);
    
}
