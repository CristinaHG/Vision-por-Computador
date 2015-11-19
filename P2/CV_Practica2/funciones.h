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
Mat estimaHomografia( Mat im1, vector<Point2f> puntosI1,vector<Point2f> puntosI2);
Mat aplicaBRISK(Mat original, vector<KeyPoint> &keypoints,Mat &descriptor, Mat salida);
Mat aplicaORB(Mat original, vector<KeyPoint> &keypoints,Mat &descriptor, Mat salida);
Mat hallaCorresp(Mat im1, Mat im2,vector<KeyPoint> kp1,vector<KeyPoint> kp2,Mat descrip1,Mat descrip2, string criterio,vector<DMatch> &coincidencias);
Mat calculaMosaico(Mat im1, Mat im2,vector<KeyPoint> keypoints1,vector<KeyPoint> keypoints2,vector<DMatch> coincidencias);
Mat calculaMosaicoMultiples(vector<Mat> imagenes,vector<vector<KeyPoint> > keypoints,vector<vector<DMatch> > coincidencias);
//Mat GeneraHomografa(Mat im1, Mat homografia);
#endif	/* FUNCIONES_H */

