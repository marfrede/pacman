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
    
    void createGameScene(GLFWwindow* pWindow); //Erstellt Feld und platziert Pacman und Geister auf dem Feld
    void start(GLFWwindow* pWindow);
    void createGameModels(GLFWwindow* pWindow);
    void createGhost(GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ);
    void createPacman(GLFWwindow* pWindow, Color primary, Color secondary, float posX, float posZ);
    
    void setGameMode(GameMode gm) {this->gamemode = gm;}
    GameMode getGameMode() {return this->gamemode;}
    
    void update(float dtime); //Updated Pacman und Geister
    void draw(const Camera camera);
    
    void checkGameOver(); //Prüfe Abbruchbedingungen - prüfe Feld pointsEmpty
    bool isGameOver() {return this->gameOver;}
    void end();
    
private:
    GameMode gamemode = GameMode::ThirdPerson;
    Field* pField;
    GhostList Ghosts;
    Pacman* pPacman;
    
    bool gameOver = true;
};

#endif /* Game_hpp */
