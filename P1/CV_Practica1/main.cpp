/* 
 * File:   main.cpp
 * Author: cris
 *
 * Created on 13 October 2015, 11:41
 */

#include <cstdlib>

#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>

#include "funciones.h"

using namespace std;
using namespace cv;

/*
 * 
 */
int main(int argc, char** argv) {

    Mat mascara = calculaMascara(1);
    cout << mascara << endl;
    Mat imagen;
    // imagen =leeimagen("basketball1.png", -1);
    //cout<<"IMAGEN SIN CONVOL"<<imagen;
    cout << "numero de canales" << imagen.channels() << endl;
    //pintaI("basketball1.png");
    imagen.convertTo(imagen, CV_32F);
    Mat vec(1, 100, CV_32F, Scalar());
    for (int i = 0; i < 10; i++) {
        vec.at<float>(i) = i + 1;
    }


    imagen = convolucion2D("dog.bmp", 5, 1);
    imshow("imagenconvol", imagen);
    waitKey(0);
    destroyWindow("imagenconvol");
    //pintaI("basketball1.png");

    return 0;
}



