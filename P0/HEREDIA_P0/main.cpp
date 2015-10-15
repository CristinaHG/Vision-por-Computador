/* 
 * File:   main.cpp
 * Author: cris
 *
 * Created on 04 October 2015, 19:20
 */

#include <cstdlib>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "funciones.h"

int main(int argc, char** argv) {
    
    vector<string> imagenes;
    
    //leeimagen("lena.jpg", 2);
    //pintaI("lena.jpg");
   
    
    //imagenes.push_back("basketball1.png");
    imagenes.push_back("lena.jpg");
    imagenes.push_back("fruits.jpg");
    //imagenes.push_back("board.jpg");
    
    pintaMI(imagenes);
     
//    Point p1,p2,p3, p4,p5;
//    p1.x=1;
//    p1.y=50;
//    
//    p2.x=100;
//    p2.y=2;
//    
//    p3.x=45;
//    p3.y=25;
//    
//    p4.x=10;
//    p4.y=3;
//    
//    p5.x=70;
//    p5.y=23;
    
    vector<Point> puntos;
    for(int i=50;i<255;i++){
        for(int j=0; j<200;j++){
            Point p;
            p.x=i;
            p.y=j;
        puntos.push_back(p);    
        } 
    }
    
    
    
    //puntos.push_back(p1);
    //puntos.push_back(p2);
    //puntos.push_back(p3);
    //puntos.push_back(p4);
    //puntos.push_back(p5);
    
    modificaPixel("lena.jpg", puntos);
    
    return 0;
}

