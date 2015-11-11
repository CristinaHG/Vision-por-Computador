/* 
 * File:   funciones.h
 * Author: cris
 *
 * Created on 9 November 2015, 22:20
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
Mat EstimaHomografia( Mat im1, vector<Point2f> puntosI1,vector<Point2f> puntosI2);
Mat AplicaBRISK(Mat original, vector<KeyPoint> keypoints,Mat descriptor, Mat salida);
Mat AplicaORB(Mat original, vector<KeyPoint> keypoints,Mat descriptor, Mat salida);
//Mat GeneraHomografa(Mat im1, Mat homografia);
#endif	/* FUNCIONES_H */

