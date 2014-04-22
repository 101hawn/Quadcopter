/* 
 * File:   LoadingRender.cpp
 * Author: nathan
 * 
 * Created on 14 April 2014, 11:02
 */



#include "LoadingRender.h"
#include "MainRender.h"
#include "SettingsRender.h"


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <inttypes.h>
#include <iostream>
#include <thread>


#if defined(_WIN32)
#include "WindowsExtensions.h"
#endif


using namespace sf;
using namespace std;
bool loaded = false;
SettingsRender* settingoverlayrender;
thread* loaderthread;
void LoadAssets()
{

    Image* image = new Image();
    image->loadFromFile("/tmp/image.jpg");
    settingoverlayrender->overlay->image = image;

    settingoverlayrender->font.loadFromFile("BS.TTF");
    sleep(seconds(1));
    loaded = true;
    sleep(seconds(3));
    settingoverlayrender->overlay->_globalvariables->currentrenderer = settingoverlayrender;

    
}
LoadingRender::LoadingRender(globals* globalvariables) : Renderer::Renderer(globalvariables) {
   
    settingoverlayrender = new SettingsRender(globalvariables);
    
}

void LoadingRender::Load() {

    
    if(! loadingfont.loadFromFile("BS.TTF"))
    {
         std::cout << "Failed to load assets";
         exit(EXIT_FAILURE);
        
    }
    loadingtext.setFont(loadingfont);
    loadingtext.setString("Loading...");
    loadingtext.setCharacterSize(120);

    
    
    if(!loadingtexture.loadFromFile("loading.png"))
    {
         std::cout << "Failed to load assets";
         exit(EXIT_FAILURE);
        
    }
    
    loadingscreen.setTexture(&loadingtexture);
   loaderthread = new thread(LoadAssets);
    
}

Clock timer;
void LoadingRender::Draw(RenderWindow& window) {
   
    loadingtext.setPosition(250,80-(drop*drop));
    
    glClearColor(0.01623f/dark,0.01316f/dark,0.07061/dark,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1,1,-1,1,1,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-3);
    glRotatef(120,1,0,0);
    glTranslatef(0,0,drop*drop);
    glRotatef(rotation,0 ,0,1);
    
    Texture::bind(&loadingtexture);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex3f(-3,-3,0);
    glTexCoord2f(0,1);glVertex3f(-3,3,0);
    glTexCoord2f(1,1);glVertex3f(3,3,0);
    glTexCoord2f(1,0);glVertex3f(3,-3,0);
    glEnd();
    
    window.pushGLStates();
    window.draw(loadingtext);
    window.popGLStates();
    



}

void LoadingRender::Input(RenderWindow& window) {
    if(Keyboard::isKeyPressed(Keyboard::Escape))
        _globalvariables->running = false;
}


void LoadingRender::Update(RenderWindow& window) {
    
    if(loaded)
    {   rotationspeed/=1.01f;
        if(rotationspeed<0.1)
            drop*=1.05;
        if(drop>2)
            dark*=1.05;
        
    }
    rotation+=rotationspeed;
        
    
}
void LoadingRender::Events(sf::Event& event)
{
    DefaultEvents(event);
    
    if(event.type == Event::Resized)
    {
       _globalvariables->height = event.size.height;
        _globalvariables->width = event.size.width;
        glViewport(0,0,_globalvariables->width,_globalvariables->height);

    }
    

}



LoadingRender::~LoadingRender() {
    delete settingoverlayrender;
    
    delete loaderthread;
}

