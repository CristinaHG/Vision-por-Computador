/* 
 * File:   main.cpp
 * Author: cris
 *
 * Created on 9 November 2015, 22:20
 */

#include <cstdlib>

#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>

#include "funciones.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {

vector<Point2f> puntos1;
vector<Point2f> puntos2;

    Mat imagen1=leeimagen("data/Tablero1.jpg", -1);
    Mat imagen2=leeimagen("data/Tablero2.jpg", -1);
    Mat homografa;
    Mat homografia=EstimaHomografia( imagen1, imagen2, puntos1,puntos2);
    warpPerspective(imagen1,homografa,homografia,homografa.size() );
 
    imshow("original 1",imagen1);
    imshow("homografia calculada",homografa);
 
    waitKey(0);
    return 0;
    return 0;
}



