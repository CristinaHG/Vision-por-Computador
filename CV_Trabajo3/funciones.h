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
Mat aplicaBRISK(Mat original, vector<KeyPoint> &keypoints,Mat &descriptor, Mat salida);
Mat aplicaORB(Mat original, vector<KeyPoint> &keypoints,Mat &descriptor, Mat salida);
Mat hallaCorresp(Mat im1, Mat im2,vector<KeyPoint> kp1,vector<KeyPoint> kp2,Mat descrip1,Mat descrip2, string criterio,vector<DMatch> &coincidencias);
Mat estimaMatrizCamara();
int calculaDeterminante(Mat matriz);
#endif	/* FUNCIONES_H */

