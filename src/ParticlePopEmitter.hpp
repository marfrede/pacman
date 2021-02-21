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
#include "ParticleEmitter.hpp"

class ParticlePopEmitter : public ParticleEmitter
{
public:
	ParticlePopEmitter(int maxParticles, Color color);
	ParticlePopEmitter(int maxParticles, BaseShader* pShader);

	void trigger(Vector pos, int quantity);

	void update(float dtime);
	void draw(const Camera Cam);

	bool empty = false;

protected:
	bool triggered = false;

};

#endif /* ParticlePopEmitter_hpp */
