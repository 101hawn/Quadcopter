/* 
 * File:   Transition.cpp
 * Author: nathan
 * 
 * Created on 16 April 2014, 11:20
 */

#include "Transition.h"
#include <cmath>

Transition::Transition() {
}

void Transition::Set(float begin, float end, int time,float power) {
    Set(begin,end,time);
    pow = power;

    
}

void Transition::Set(float begin, float end, int time) {


     _begin = begin;
    _end = end;
    _currenttime=0;
    _endtime= time;
}

void Transition::Set(float end, int time) {
    Set(Value(),end,time);
}

void Transition::Set(float end) {
    Set(end,defaultendtime);
}



float Transition::Value() {
    if(_currenttime>_endtime)
    {
        return _end;
        
    }
    float cache = ((_end-_begin)*(_currenttime/((float)_endtime)));
    float value = _begin + (std::pow (cache,pow)/std::pow(cache/(_currenttime/((float)_endtime)),pow-1));
    return value;
          
}

void Transition::Update() {

  _currenttime++;
    
}

Transition::~Transition() {
}

