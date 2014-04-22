/* 
 * File:   SettingsRender.cpp
 * Author: nathan
 * 
 * Created on 15 April 2014, 12:02
 */


#include "SettingsRender.h"
#include "Transition.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace sf;
using namespace std;
Transition fadetext;
Transition filter;
Transition pitch;
RectangleShape filtershape;
int optionlength = 9;
vector<SettingItem> options(optionlength);
int selectedoption = 0;
Text numberbox;


string fix( float x, int p )
{
    ostringstream strout ;
    strout << fixed << setprecision(p) << x ;
    string str = strout.str() ;
    size_t end = str.find_last_not_of( '0' ) + 1 ;
    str  = str.erase( end ) ;
    if(str[str.size()-1]=='.')
        str = str.erase(str.size()-1);
    return str;
}
void Add(float& num,char toadd)
{
    
    
    stringstream ss;
   
    ss << fix(num,8);
    
    if(ss.str().length()>6&&toadd!='\b')
        return;
    if(toadd=='\b')
    {
        string s = ss.str();
        if(s.size()>0)
        {
            s= s.substr(0,s.size()-1); 
            
        }
        ss.str("");
        ss<<s;
        
    }
    else
    {
        ss << toadd;
    }
    double newnum = 0;
    
    ss >> newnum;
    num = newnum;
    
}


SettingsRender::SettingsRender(globals* globalvariables) : Renderer::Renderer(globalvariables) {
    overlay = new MainRender(globalvariables);
    

}

void SettingsRender::Load() {
    overlay->EnsureLoaded();
    
    numberbox.setFont(font);
    numberbox.setPosition(300,500);
    
    
    pitch.defaultendtime = 10;
    
    options[0].item = &overlay->deathchance;
    options[0].text.setString("Chance of particle death");
    options[0].y.Set(-30);
    options[0].y.defaultendtime = 10;    
    options[1].item = &overlay->depth;
    options[1].text.setString("Depth of particles");
    options[1].y.Set(-30);
    options[1].y.defaultendtime = 10;
    options[2].item = &overlay->camfar;
    options[2].text.setString("Far viewing distance");
    options[2].y.Set(-30);
    options[2].y.defaultendtime = 10;
    options[3].item = &overlay->fixedparticles;
    options[3].text.setString("Number of fixed particle");
    options[3].y.Set(-30);
    options[3].y.defaultendtime = 10;
    options[4].item = &overlay->maxparticles;
    options[4].text.setString("Max number of particles alive");
    options[4].y.Set(-30);
    options[4].y.defaultendtime = 10;
    options[5].item = &overlay->minresurrectchance;
    options[5].text.setString("Chance of particle respawning");
    options[5].y.Set(-30);
    options[5].y.defaultendtime = 10;
    options[6].item = &overlay->camnear;
    options[6].text.setString("Near viewing distance");
    options[6].y.Set(-30);
    options[6].y.defaultendtime = 10;
    options[7].item = &overlay->pointsize;
    options[7].text.setString("Point size");
    options[7].y.Set(-30);
    options[7].y.defaultendtime = 10;
    options[8].item = &overlay->redrawchance;
    options[8].text.setString("Chance of particle redraw");
    options[8].y.Set(-30);
    options[8].y.defaultendtime = 10;
    _font = font;
    text.setFont(_font);
    text.setString("Options");
    
    filtershape.setSize(Vector2f(1000,1000));
    
    fadetext.Set(0,255,100);
    filter.Set(0);
}
void SettingsRender::Draw(sf::RenderWindow& window) {

    window.clear(Color::Black);
    text.setColor(Color(255,255,255,abs(fadetext.Value())));
    text.setPosition(700,0);

    
    
    filtershape.setFillColor(Color(10,0,10,filter.Value()));
    
    
    if(!(filter.Value()==255))
    {
        window.resetGLStates();
        overlay->Draw(window);
        
    }
    window.draw(filtershape);
    
    for(int i = 0;i<optionlength;i++)
    {
        if((selectedoption+i)%optionlength==3)
        {
            options[i].text.setColor(Color(255,255,255,options[i].fade.Value()));
            numberbox.setColor(Color(255,255,255,options[i].fade.Value()));
            Vector2f pos = options[i].text.getPosition();
            pos.x+= 400;
            numberbox.setPosition(pos);
            stringstream ss;
            ss << fix(*(options[i].item),10);
            numberbox.setString(ss.str());
            window.draw(numberbox);
            
        }
        else
        {
            options[i].text.setColor(Color(100,0,100,options[i].fade.Value()));            
            
        }
        
        options[i].text.setFont(font);        
        float y = options[i].y.Value();
        
        options[i].text.setPosition(50,y);
        window.draw(options[i].text);

    }
    
    
    window.draw(text);
    
}

void SettingsRender::Events(sf::Event& event) {

    
    overlay->Events(event);
    if(event.type== Event::MouseButtonPressed)
    {
        if(event.mouseButton.button == Mouse::Left)
        {
            if(text.getGlobalBounds().contains(event.mouseButton.x,event.mouseButton.y))
            {
                
                overlayactive = !overlayactive;
                if(overlayactive)
                {
                    fadetext.Set(-255,10);
                    filter.Set(255,5);
                    for(int i = 0;i<optionlength;i++)
                    {
                        options[i].y.pow = 2;
                        options[i].y.Set((selectedoption+i)%optionlength*50,1+(selectedoption+i)%optionlength*100,10);
                        options[i].fade.defaultendtime = 10;                                          
                        options[i].fade.Set(255);
                    }
                    pitch.Set(90);
                }
                else
                {
                    fadetext.Set(255,10);
                    filter.Set(0,5);
                    for(int i = 0;i<optionlength;i++)
                    {
                        options[i].fade.defaultendtime = 5;                       
                        options[i].fade.Set(0);
                    } 
                    pitch.Set(0);
                }
            }
        }
        
        
    }
    
        if(event.type == Event::KeyReleased)
        {
            while(3-selectedoption<0)
            {
                selectedoption-=optionlength;
                
            }
            while(3-selectedoption>optionlength)
            {
                selectedoption+=optionlength;
                
            }            
            
            
            
            if(overlayactive)
            {
                switch(event.key.code)
                {
                    case Keyboard::Num0:
                        Add(*options[3-selectedoption].item,'0');
                        break;
                    case Keyboard::Num1:
                        Add(*options[3-selectedoption].item,'1');
                        break;
                    case Keyboard::Num2:
                        Add(*options[3-selectedoption].item,'2');
                        break;
                    case Keyboard::Num3:
                        Add(*options[3-selectedoption].item,'3');
                        break;
                    case Keyboard::Num4:
                        Add(*options[3-selectedoption].item,'4');
                        break;
                    case Keyboard::Num5:
                        Add(*options[3-selectedoption].item,'5');
                        break;
                    case Keyboard::Num6:
                        Add(*options[3-selectedoption].item,'6');
                        break;
                    case Keyboard::Num7:
                        Add(*options[3-selectedoption].item,'7');
                        break;
                    case Keyboard::Num8:
                        Add(*options[3-selectedoption].item,'8');
                        break;
                    case Keyboard::Num9:
                        Add(*options[3-selectedoption].item,'9');
                        break;
                    case Keyboard::BackSpace:
                        Add(*options[3-selectedoption].item,'\b');                        
                        break;
                    case Keyboard::Period:
                        Add(*options[3-selectedoption].item,'.');                        
                        break;                        
                    
                }
                
                
            }
            while(selectedoption<0)
            {
                selectedoption+=optionlength;
                
            }
            while(selectedoption>optionlength)
            {
                selectedoption-=optionlength;
                
            }            
            
            

            if(event.key.code == Keyboard::Up)
            {
                selectedoption--;
           
            } 
            
                    
            if(event.key.code == Keyboard::Down)
            {
                selectedoption++;
            }
            
            for(int i = 0;i<optionlength;i++)
            {
            options[i].y.pow = 1;
            options[i].y.Set((1+selectedoption+i)%optionlength*100,10);
            }
            
        }
    
    
}

void SettingsRender::Input(sf::RenderWindow& window) {
    if(!overlayactive)
        overlay->Input(window);
}


void SettingsRender::Update(sf::RenderWindow& window) {
    
    
    fadetext.Update();
    filter.Update();
    for(int i = 0;i<optionlength;i++)
    {

        options[i].y.Update();
        options[i].fade.Update();

    }
    pitch.Update();
    overlay->pitch= pitch.Value();
    if(fadetext.Value()<0)
    {

        text.setString("Exit");
        

    }
    else
    {

        text.setString("Options");
        overlay->Update(window);

    }
    
}


SettingsRender::~SettingsRender() {
    delete overlay;
}

