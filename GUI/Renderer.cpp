/* 
 * File:   Renderer.cpp
 * Author: nathan
 * 
 * Created on 13 April 2014, 21:58
 */

#include "Renderer.h"
#include <SFML/Window.hpp>

Renderer::Renderer(globals* globalvariables) {
_globalvariables = globalvariables;
}


void Renderer::Tick(sf::RenderWindow& window)
{
    EnsureLoaded();
    sf::Event event;
    while(window.pollEvent(event))
    {
        Events(event);
    }
    
    Input(window);
    Update(window);
    Draw(window);
    
    window.display();
    

}

void Renderer::Events(sf::Event& event) {
    DefaultEvents(event);
}

void Renderer::DefaultEvents(sf::Event& event) {
    if(event.type == sf::Event::Closed)
    {
        _globalvariables->running = false;

    }
}

void Renderer::EnsureLoaded() {
    if(!renderloaded)
    {
        Load();
        renderloaded = true;
    }
}


