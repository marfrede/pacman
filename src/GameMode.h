//
//  GameMode.h
//  CGXcode
//
//  Created by [privat] Frederik Lau on 16.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef GameMode_h
#define GameMode_h

enum class GameMode {
    FirstPerson,
    ThirdPerson,
    Debug
};
#define gamemodeToString(o) (o == GameMode::FirstPerson? "FirstPerson" : (o == GameMode::ThirdPerson? "ThirdPerson" : "Debug")))

#endif /* GameMode_h */
