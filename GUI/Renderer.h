/* 
 * File:   Renderer.h
 * Author: nathan
 *
 * Created on 13 April 2014, 21:58
 */

#ifndef RENDERER_H
#define	RENDERER_H

#include <SFML/Graphics.hpp>



struct globals;

class Renderer {
public:
    Renderer(globals*);
    void EnsureLoaded();
    virtual void Update(sf::RenderWindow&) =  0;
    virtual void Draw(sf::RenderWindow&) = 0;
    virtual void Input(sf::RenderWindow&) = 0;
    virtual void Events(sf::Event&);
    void DefaultEvents(sf::Event&);
    void Tick(sf::RenderWindow&);
    globals* _globalvariables;

protected:
    virtual void Load() = 0;
private:
    bool renderloaded;
   
};
struct globals
{
    bool running;
    Renderer* currentrenderer;
    float height = 0,width = 0;

};
#endif	/* RENDERER_H */

