//
//  ParticlePopEmitter.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "ParticlePopEmitter.hpp"

ParticlePopEmitter::ParticlePopEmitter(int maxParticles, Color color) {
	this->color = color;
	for (int i = 0; i < maxParticles; i++) {
		Particle* p = new Particle(this->position, Vector(0, 1, 0), 45, 200, 1);
		ConstantShader* cShader = new ConstantShader();
		float r = this->color.R * 255 + ((rand() % 60) - 30);
		float g = this->color.G * 255 + ((rand() % 60) - 30);
		float b = this->color.B * 255 + ((rand() % 60) - 30);
		if (r < 0) r = 0;
		if (g < 0) g = 0;
		if (b < 0) b = 0;
		if (r > 255) r = 255;
		if (g > 255) g = 255;
		if (b > 255) r = 255;
		cShader->color(Color(r / 255, g / 255, b / 255));
		p->shader(cShader, true);
		this->particles.push_back(p);
	}
}
ParticlePopEmitter::ParticlePopEmitter(int maxParticles, BaseShader* pShader) {
	this->color = color;
	for (int i = 0; i < maxParticles; i++) {
		Particle* p = new Particle(this->position, Vector(0, 1, 0), 45, 200, 1);
		p->shader(pShader, true);
		this->particles.push_back(p);
	}
}

void ParticlePopEmitter::trigger(Vector pos, int quantity) {
	int tmpQuantity = quantity;
	//Exception wenn alle aktiv sind
	for (std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
		if ((*it)->isAlive() == false && tmpQuantity > 0) {
			(*it)->reset(pos);
			tmpQuantity--;
		}
	}
}

void ParticlePopEmitter::update(float dtime) {
	for (std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
		if ((*it)->isAlive() == true) {
			(*it)->update(dtime);
		}
	}
}

void ParticlePopEmitter::draw(const Camera Cam) {
	for (std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
		if ((*it)->isAlive() == true) {
			(*it)->draw(Cam);
		}
	}
}

void ParticlePopEmitter::end() {
	for (std::list<Particle*>::iterator it = this->particles.begin(); it != this->particles.end(); it++) {
		delete (*it);
	}
	this->particles.clear();
}