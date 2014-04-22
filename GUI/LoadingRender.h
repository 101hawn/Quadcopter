/* 
 * File:   LoadingRender.h
 * Author: nathan
 *
 * Created on 14 April 2014, 11:02
 */

#ifndef LOADINGRENDER_H
#define	LOADINGRENDER_H

#include<SFML/Graphics.hpp>

#include "Renderer.h"

class LoadingRender : public Renderer{
public:
    LoadingRender(globals*);

    virtual void Load();

    virtual void Draw(sf::RenderWindow&);
    virtual void Input(sf::RenderWindow&);
    virtual void Update(sf::RenderWindow&);
    virtual void Events(sf::Event&);
    
    


    virtual ~LoadingRender();
private:

    sf::RectangleShape loadingscreen;
    sf::Texture loadingtexture;
    sf::View view,fullview;
    sf::Text loadingtext;
    sf::Font loadingfont;
    float drop = 0.1f;
    float dark = 0.1f;
    float rotation = 0;
    float rotationspeed= 0.2f;
};

#endif	/* LOADINGRENDER_H */

