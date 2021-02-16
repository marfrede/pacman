//
//  Pacman.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 01.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef Pacman_hpp
#define Pacman_hpp

#include "TriangleBoxModel.h"
#include "PhongShader.h"
#include "GameCharacter.hpp"
#include "EgoCam.hpp"
#include <stdio.h>

class Pacman : public GameCharacter
{
public:

	/**
    * create a Pacman at given position
    * @param posX (min:0, max:planeWidth) x position on the field
    * @param posZ (min:0, max:planeDepth) z position on the field
    * @param pPhongShader to use for the pacman
	*/
	Pacman(int posX, int posZ, const char* ModelFile, bool FitSize);
    
    void draw(const Camera Cam);

    void setArrow(BaseModel* arrow) {this->arrow = arrow;}

	//void update(float dtime);
    void moveSubs();

	/**
	 STEERING
	 */
	 /**
	  * steer object
	  * @param dtime deltatime
	  */
	void steer(float dtime);

	//void setEgoCam(EgoCam cam){paccam=&cam;}
	//EgoCam* paccam;
	//void movePaccam();

private:

    BaseModel* arrow;

};

#endif /* Pacman_hpp */
