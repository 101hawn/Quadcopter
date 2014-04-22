/* 
 * File:   MainRender.cpp
 * Author: nathan
 * 
 * Created on 13 April 2014, 21:57
 */

#include "MainRender.h"



using namespace sf;

Vector3f  MainRender::worldtopixel (Vector3f world)
{
   
    float scale;
    if(_globalvariables->height>_globalvariables->width)
        scale = _globalvariables->height;
    else
        scale = _globalvariables->width;

    Transform transform = Transform::Identity;
    transform.rotate(yaw);


    float x = world.x+posx;
    float y = (-world.y)-posy;
    float z = world.z+posz;		

    Vector2f point = transform.transformPoint(x,z);
    z=point.y;


    x=point.x*size.x*scale/(depth*_globalvariables->width);
    y=y*size.y*scale/(depth*_globalvariables->height);
    x+=size.x/2.f;
    y+=size.y/2.f;


    

    

    return Vector3f(x,y,z);
}
Vector3f MainRender::pixeltoworld(Vector3f pixel)
{
    float scale;
    if(_globalvariables->height>_globalvariables->width)
        scale = _globalvariables->height;
    else
        scale = _globalvariables->width;
        
    
    
    Transform transform = Transform::Identity;
    transform.rotate(-yaw);
    float x = pixel.x;
    float y = pixel.y;
    float z = pixel.z;
    x -= size.x/2.f;
    y -= size.y/2.f;


    x=(x/size.x)*((_globalvariables->width/scale)*depth);
    y=(y/size.y)*((_globalvariables->height/scale)*depth);

    Vector2f point = transform.transformPoint(x,z);


    return Vector3f(point.x -posx,-y - posy,point.y - posz);
}
float MainRender::random_float(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

MainRender::MainRender(globals* globalvariables) : Renderer(globalvariables) {
    
    

    std::vector<particle*> overrideparticles(maxparticles);
    particles = overrideparticles;
    for(int i = 0;i<maxparticles;i++)
    {
        particles[i]= new particle;
        particles[i]->dead = true;
        
    }
    
    
   
    
}

void MainRender::Load() {
    _image = image;
}

void MainRender::Draw(RenderWindow& window)
{
    window.resetGLStates();
    
  

        float ratio = _globalvariables->width / (float) _globalvariables->height;
        glClearColor(0,0,0,0);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum (-ratio, ratio, -1.f, 1.f, camnear, camfar);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(-yaw,0.f,1.f,0.f);        
        glTranslatef(posx,posy,posz);
        glRotatef(pitch,1.f,0.f,0.f);

        glPointSize(pointsize);
        glBegin(GL_POINTS);
        for(int i = 0;i<maxparticles;i++)
        {
            if(!particles[i]->dead)
            {
                glColor3f(particles[i]->r,particles[i]->g,particles[i]->b);
                glVertex3f(particles[i]->x,particles[i]->y,particles[i]->z);
            }
        }
                
                        
        glEnd();  
        
    
}
void MainRender::Input(RenderWindow& window)
{
    
   if(Keyboard::isKeyPressed(Keyboard::Up))
            {
                Transform rotate = Transform::Identity;
                rotate.rotate(-yaw);
                Vector2f direction = rotate.transformPoint(0,0.1f);
                
                posx+=direction.x;
                posz+=direction.y;
            }
            if(Keyboard::isKeyPressed(Keyboard::Down))
            {
                
                Transform rotate = Transform::Identity;
                rotate.rotate(-yaw);
                Vector2f direction = rotate.transformPoint(0,-0.1f);
                
                posx+=direction.x;
                posz+=direction.y;
                
            }
            if(Keyboard::isKeyPressed(Keyboard::Right))
            {
                
                yaw-=2.f;
                
            }
            if(Keyboard::isKeyPressed(Keyboard::Left))
            {
                
                yaw+=2.f;
                
            }
            if(Keyboard::isKeyPressed(Keyboard::PageUp))
            {
                
                posy-=0.1f;
                
            }
            if(Keyboard::isKeyPressed(Keyboard::PageDown))
            {
                
                posy+=0.1f;
                
            }
            if(Keyboard::isKeyPressed(Keyboard::Right))
            {
                
                yaw-=2.f;
                
            }
            if(Keyboard::isKeyPressed(Keyboard::Left))
            {
                
                yaw+=2.f;
                
            }
            if(Keyboard::isKeyPressed(Keyboard::PageUp))
            {
                
                posy-=0.1f;
                
            }
            if(Keyboard::isKeyPressed(Keyboard::Escape))
            {
                
                _globalvariables->running = false;
                
            }
              
    
    
    
}
void MainRender::Update(RenderWindow& window)
{
    if(minresurrectchance<resurrectchance-1)
        resurrectchance-=(resurrectchance-minresurrectchance)/10;
    else if(minresurrectchance<resurrectchance)
        resurrectchance= minresurrectchance;
        








    



    if(timer.getElapsedTime().asMicroseconds()>100)
    {

        timer.restart();



    }




    size = _image->getSize();      


    for(int i = 0;i<maxparticles;i++)
    {
        if((!particles[i]->dead && random_float(0,deathchance)<1)||(i<size.x*size.y/(1/fixedparticles)))
        {
            particles[i]->dead = true;
        }
        if(particles[i]->dead )
        {
            if( random_float(0,resurrectchance)<1||(i<size.x*size.y/(1/fixedparticles)))
            {



                float x= i*17%(size.x-1);
                float y= i*1%(size.y-1);
                float z  = -4.f;

                Vector3f pixel = Vector3f(x,y,z);
                Vector3f world = pixeltoworld(pixel);

                Color color = _image->getPixel(pixel.x,pixel.y);

                particles[i]->r = color.r/255.f;
                particles[i]->g = color.g/255.f;
                particles[i]->b = color.b/255.f;


                particles[i]->x = world.x;
                particles[i]->y = world.y;
                particles[i]->z = world.z;
                particles[i]->dead = false;
            }
        }
        else if( random_float(0,redrawchance)<1||(i<size.x*size.y/(1/fixedparticles)))
        {
            Vector3f world = Vector3f(particles[i]->x,particles[i]->y,particles[i]->z);
            Vector3f pixel = worldtopixel(world);
            
            if(0<pixel.x&&pixel.x<size.x-1&&0<pixel.y&&pixel.y<size.y-1)
            {
                if(pixel.z>-4.5f&&pixel.z<-3.5f)
                {
                    Color color = _image->getPixel(pixel.x,pixel.y);

                    particles[i]->r = color.r/255.f;
                    particles[i]->g = color.g/255.f;
                    particles[i]->b = color.b/255.f;
                    
                }
                else
                {
                    particles[i]->dead= true;
                }
                

            }



        }

     }



}
void MainRender::Events(Event& event)
{
    DefaultEvents(event);
    


    if(event.type == Event::Resized)
    {
        _globalvariables->height = event.size.height;
        _globalvariables->width = event.size.width;
        glViewport(0,0,_globalvariables->width,_globalvariables->height);

    }
}

MainRender::~MainRender() {
    for(int i = 0;i<maxparticles;i++)
    {
        delete particles[i];
        
    }
}

