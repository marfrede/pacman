//
//  ParticlePopEmitter.cpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#include "ParticlePopEmitter.hpp"

ParticlePopEmitter::ParticlePopEmitter(int maxParticles, Color color) : ParticleEmitter(maxParticles, color) {}
ParticlePopEmitter::ParticlePopEmitter(int maxParticles, BaseShader* pShader) : ParticleEmitter(maxParticles, pShader) {}

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
