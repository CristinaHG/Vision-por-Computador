/* 
 * File:   funciones.h
 * Author: cris
 *
 * Created on 13 October 2015, 11:52
 */

#ifndef FUNCIONES_H
#define	FUNCIONES_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std; 
using namespace cv; 

//void my_imGaussConvol(Mat& im, Mat& maskCovol, Mat& out);
//cáculo del vector máscara

Mat leeimagen(string filename, int flagColor);
void pintaI(string im);
Mat calculaMascara(float sigma);
Mat calculaConvol1D_columnas(Mat imagenEntrada, Mat mascara, int contorno);
Mat calculaConvol1D_filas(Mat imagenEntrada, Mat mascara, int contorno);
Mat convolucion2D(string imagenEntrada, float sigma, int contorno);
#endif	/* FUNCIONES_H */

