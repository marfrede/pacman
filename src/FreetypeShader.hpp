//
//  FreetypeShader.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 18.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef FreetypeShader_hpp
#define FreetypeShader_hpp

#include <stdio.h>
#include "BaseShader.h"

class FreetypeShader : public BaseShader
{
public:
    FreetypeShader();
    void color( const Color& c);
    const Color& color() const { return Col; }
    virtual void activate(const BaseCamera& Cam) const;
private:
    Color Col;
    GLint ColorLoc;
    
};

#endif /* FreetypeShader_hpp */
