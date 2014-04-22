/* 
 * File:   main.cpp
 * Author: nathan
 *
 * Created on 11 April 2014, 16:04
 */
#include <math.h>
#include <cstdlib>  
#include <stdio.h>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <thread>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <algorithm>

#include "MainRender.h"
#include "LoadingRender.h"


using namespace std;
using namespace sf;
/*
 * 
 */




globals* globalvariables = new globals;


RenderWindow window;


bool connected = false;
/*
void Capture()
{
    

    Ftp ftp;
    ftp.connect(IpAddress::LocalHost);
    Ftp::Response response =  ftp.login();
 
    
    string remoteFile = "/image.jpg";
    string localPath = "/tmp";
    string listingPath = "image.jpg";


    Image* images[2];
    int index = 0;
    images[0] = new Image;
    images[1] = new Image;
    response = ftp.download(remoteFile,localPath,Ftp::TransferMode::Binary);
    int fd = open("/tmp/image.jpg",O_RDWR);
    struct stat sbuf;
    void* data;
    stat("/tmp/image.jpg",&sbuf);
    data = mmap((caddr_t)0,sbuf.st_size,PROT_READ,MAP_SHARED,fd,0);

    while(globalvariables->running)
    {
        vector<string> listing = ftp.getDirectoryListing(".").getListing();
        
        if(find(listing.begin(),listing.end(),listingPath)!=listing.end())
        {
            Ftp::Response response = ftp.renameFile(remoteFile,remoteFile+".lock");
             if(response.isOk())
            {
            response = ftp.download(remoteFile+".lock",localPath,Ftp::TransferMode::Binary);
            ftp.renameFile(remoteFile+".lock",remoteFile);
        
        
        
                if(response.isOk())
                {


                    images[index]->loadFromMemory(data,sbuf.st_size); 
                    mainrender.image = images[index];
                    index++;
                    if(index>1)
                        index = 0;
                     connected = true;
                }
            }
        }
        sleep(milliseconds(100));
    }   
    
    delete images[0];
    delete images[1];
    
         
    
}

*/

int main(int argc, char** argv) {
    
    

    
    globalvariables->running = true;

    
    
    window.create(VideoMode(800,600),"Quadcopter Project");
    
    window.setVerticalSyncEnabled(true);
    

    
   
    
     
    //thread CaptureThread(Capture);
    
    LoadingRender loadingrender(globalvariables);
    loadingrender.Load();
    
    globalvariables->currentrenderer = &loadingrender;
    Clock fps;
    while (globalvariables->running)
    {
        globalvariables->currentrenderer->Tick(window);
        
        

        
    }

    delete globalvariables;
    window.close();
    exit(EXIT_SUCCESS);



}

