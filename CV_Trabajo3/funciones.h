/* 
 * File:   funciones.h
 * Author: cris
 *
 * Created on 04 January 2016, 16:38
 */

#ifndef FUNCIONES_H
#define	FUNCIONES_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std; 
using namespace cv; 

Mat leeimagen(string filename, int flagColor);
void pintaI(string im);
Mat estimaMatrizCamara();
int calculaDeterminante(Mat matriz);
#endif	/* FUNCIONES_H */

