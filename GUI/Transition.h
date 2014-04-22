/* 
 * File:   Transition.h
 * Author: nathan
 *
 * Created on 16 April 2014, 11:20
 */

#ifndef TRANSITION_H
#define	TRANSITION_H

#include <SFML/Graphics.hpp>



class Transition {
public:
    Transition();
    float Value();
    void Update();
    void Set(float,float,int,float);
    void Set(float,float,int);
    void Set(float,int);
    void Set(float);
    virtual ~Transition();
    int defaultendtime = 0;
    float pow = 1;
private:
    float _begin = 0, _end = 0;
    int _currenttime = 0, _endtime = 0;
    

};

#endif	/* TRANSITION_H */

