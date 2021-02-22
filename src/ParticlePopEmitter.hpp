//
//  ParticlePopEmitter.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 15.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef ParticlePopEmitter_hpp
#define ParticlePopEmitter_hpp

#include <stdio.h>
#include <list>
#include "Particle.hpp"

class ParticlePopEmitter
{
public:
	ParticlePopEmitter(int maxParticles, Color color);
	ParticlePopEmitter(int maxParticles, BaseShader* pShader);

	void trigger(Vector pos, int quantity);

	void update(float dtime);
	void draw(const Camera Cam);
	void end();

	bool empty = false;

protected:
	bool triggered = false;

	Vector position;
	Color color;

	std::list<Particle*> particles;

};

#endif /* ParticlePopEmitter_hpp */
