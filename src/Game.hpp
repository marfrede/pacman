//
//  Game.hpp
//  CGXcode
//
//  Created by [privat] Frederik Lau on 16.02.21.
//  Copyright © 2021 Philipp Lensing. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "GameMode.h"
#include "Ghost.hpp"
#include "Pacman.hpp"
#include <stdio.h>
#include <list>

class Game
{
public:
    typedef std::list<Ghost*> GhostList;
    
    Game();
    
    BaseModel* getPacman();
    
    void start(Field* pField, GLFWwindow* pWindow); //Platziert Pacman und Geister auf dem Feld
    void createGameModels(Field* pField, GLFWwindow* pWindow);
    void createGhost(Field* pField, GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ);
    void createPacman(Field* pField, GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ);
    
    void update(float dtime); //Updated Pacman und Geister
    void draw(const Camera camera);
    
    bool gameOver(Field* pField); //Prüfe Abbruchbedingungen - prüfe Feld pointsEmpty
    void end();
    
private:
    GhostList Ghosts;
    Pacman* pPacman;
};

#endif /* Game_hpp */
