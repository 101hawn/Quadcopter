/* 
 * File:   SettingsRender.h
 * Author: nathan
 *
 * Created on 15 April 2014, 12:02
 */

#ifndef SETTINGSRENDER_H
#define	SETTINGSRENDER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Renderer.h"
#include "MainRender.h"
#include "Transition.h"

struct SettingItem
{
    float* item;
    Transition y;
    Transition fade;
    sf::Text text;
    
};

class SettingsRender : public Renderer {
public:
    SettingsRender(globals*);

    virtual void Load();

    virtual void Draw(sf::RenderWindow&);
    virtual void Input(sf::RenderWindow&);
    virtual void Update(sf::RenderWindow&);
    virtual void Events(sf::Event&);
    
    MainRender* overlay;
    sf::Font font;
    
    virtual ~SettingsRender();
private:
    bool overlayactive = false;
    sf::Text text;
    sf::Font _font;
            

};

#endif	/* SETTINGSRENDER_H */

