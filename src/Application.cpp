//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "wall.h"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "model.h"
#include "ShaderLightmapper.h"



#include "GUI.hpp"
#include <string>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#define TEXTURE_DIRECTORY "../../assets/texture/"
#else
#define ASSET_DIRECTORY "../assets/"
#define TEXTURE_DIRECTORY "../assets/texture/"
#endif

#ifdef WIN32
#define FONT_DIRECTORY "../../assets/fonts/"
#else
#define FONT_DIRECTORY "../assets/fonts/"
#endif

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    std::pair<int, int> Size;   // Size of glyph
    std::pair<int, int> Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;

using namespace std;

Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), Paccam(pWin, NULL), pModel(NULL), ShadowGenerator(2048, 2048)
{
	createScene();
}
void Application::start()
{
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
}

void Application::update(float dtime)
{
    //Menüauswahl
    //Bei Start
    
    pGame->update(dtime);
    
    if(this->pGame->isGameOver()) {
        
        std::cout << "GAME IS OVER!" << std::endl;
        this->pGame->start(pWindow);
        
    }
        
    if (pGame->getGameMode() == GameMode::FirstPerson) {
        Paccam.update();
    }
    else {
        Cam.update();
    }
	
}

void Application::draw()
{
	ShadowGenerator.generate(Models);

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models

    Camera currentCam = Cam;
    if(pGame->getGameMode() == GameMode::FirstPerson) {
        currentCam = Paccam;
    }
    
    this->pGame->draw(currentCam);
    
    for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
    {
        (*it)->draw(currentCam);
    }

	ShaderLightMapper::instance().deactivate();
    
    //TESTING FREETYPE
    
    FT_Library  library;
    FT_Face face;
    
    FT_Error error = FT_Init_FreeType( &library );
    if ( error )
    {
        std::cout << "ERROR1" << std::endl;
    }
    
    error = FT_New_Face(library, FONT_DIRECTORY "BebasNeue-Regular.ttf", 0, &face);
    
    std::cout << FONT_DIRECTORY "BebasNeue-Regular.ttf" << std::endl;
    
    if ( error == FT_Err_Unknown_File_Format )
    {
        std::cout << "ERROR2" << std::endl;
    }
    else if ( error )
    {
        std::cout << "ERROR3" << std::endl;
    }
    
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    for (unsigned char c = 0; c < 128; c++) {
        
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        
        FT_UInt glyph_index = FT_Get_Char_Index(face, 0x1F028);
        
        FT_Load_Glyph(face, glyph_index, 0);
        
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        
        //GUI* gui = new GUI();
        //FT_Bitmap bitmap = gui->drawText();
        
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        std::pair<int, int> Size;
        Size.first = face->glyph->bitmap.width;
        Size.second = face->glyph->bitmap.rows;
        std::pair<int, int> Bearing;
        Bearing.first = face->glyph->bitmap_left;
        Bearing.second = face->glyph->bitmap_top;
        Character character = {
            texture,
            Size,
            Bearing,
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    unsigned int VAO, VBO;

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    ConstantShader shader = ConstantShader();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    std::string text = "Hallo";

    // iterate through all characters
    std::string::const_iterator c;
    
    float x = 150;
    float y = 150;
    float scale = 100;
    
    for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = x  * scale;
            float ypos = y * scale;

            float w = scale;
            float h = scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    
    // activate corresponding render state
    //Shader shader("vstext.glsl", "fstext.glsl");
    //shader;
    //Shader
    ConstantShader* cShader = new ConstantShader();
    Color color = Color(1,1,1);
        glUniform3f(glGetUniformLocation(cShader->openGLProgramID(), "textColor"), color.R, color.G, color.B);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.first * scale;
            float ypos = y - (ch.Size.second - ch.Bearing.second) * scale;
            
            xpos = 150;
            ypos = 150;
            

            float w = ch.Size.first * scale;
            float h = ch.Size.second * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            
            std::cout << "xpos: " << xpos << std::endl;
            std::cout << "ypos: " << ypos << std::endl;
            
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    
    //TESTING FREETYPE
    
	// 3. check once per frame for opengl errors
	GLenum Error = glGetError();
	assert(Error == 0);
}
void Application::end()
{
    this->pGame->end();

	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it) {
		delete* it;
	}
	this->Models.clear();
}

void Application::createScene()
{
	Matrix m, n;

	// SKY
	//pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	//pPhongShader = new PhongShader();
	//pModel->shader(pPhongShader, true);
	//pModel->shadowCaster(false);
	//Models.push_back(pModel);
    
    pGame = new Game();
    this->pGame->createGameScene(pWindow);
    this->Paccam.setObj(pGame->getPacman());
    Cam.update();

}
