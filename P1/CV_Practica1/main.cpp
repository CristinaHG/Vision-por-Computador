/* 
 * File:   main.cpp
 * Author: cris
 *
 * Created on 13 October 2015, 11:41
 */

#include <cstdlib>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "funciones.h"

using namespace std;
/*
 * 
 */
int main(int argc, char** argv) {

    Mat mascara=calculaMascara(3);
    cout<<mascara<<endl;
    
    
    return 0;
}



