/* 
 * File:   funciones.h
 * Author: cris
 *
 * Created on 13 October 2015, 11:52
 */
using namespace cv;
using namespace std; 

#ifndef FUNCIONES_H
#define	FUNCIONES_H

void my_imGaussConvol(Mat& im, Mat& maskCovol, Mat& out);
//cáculo del vector máscara
Mat calculaMascara(float sigma);
#endif	/* FUNCIONES_H */

