/* 
 * File:   MainRender.h
 * Author: nathan
 *
 * Created on 13 April 2014, 21:57
 */

#ifndef MAINRENDER_H
#define	MAINRENDER_H


#include "Renderer.h"
#include <GL/gl.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

struct particle
{
    float x,y,z;
    float r,g,b;
    bool dead ;
    
    
};
class MainRender : public Renderer 
{
public:
    MainRender(globals*);

    virtual void Load();

    virtual void Draw(sf::RenderWindow&);
    virtual void Update(sf::RenderWindow&);
    virtual void Input(sf::RenderWindow&);
    virtual void Events(sf::Event&);

    virtual ~MainRender();

    
    /*int maxparticles = 100000;
    float depth = 7;
    int pointsize = 2;
    float near = 1;
    float far = 100;
    float deathchance = 100;
    float redrawchance = 10;
    */
    sf::Image* image;
    
    float maxparticles = 100000;
    float depth = 6;
    float pointsize = 2;
    float camnear = 1;
    float camfar = 100;
    float deathchance = 30;
    float redrawchance = 2;
    float minresurrectchance = 1;
    float fixedparticles = 0.f;
    float pitch = 0;
    
    float yaw= 0,posx = 0,posy = 0, posz = 0;

private:
    sf::Image* _image;
    sf::Vector3f pixeltoworld(sf::Vector3f);
    sf::Vector3f worldtopixel(sf::Vector3f);
    float random_float(float a, float b);
   
    sf::Vector2u size;
    std::vector<particle*> particles;
    sf::Clock timer;
    float resurrectchance = 10000;


};

#endif	/* MAINRENDER_H */

