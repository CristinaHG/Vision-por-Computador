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
Mat calculaFundamental( vector<KeyPoint> keypointsIm1, vector<KeyPoint> keypointsIm2, vector<Point2f> &puntosIm1, vector<Point2f> &puntosIm2, vector<DMatch> coincidencias);
Mat dibujaEpipolares(Mat imagen1, Mat imagen2,vector<Vec3f> lineas, vector<Point2f> puntosI1, vector<Point2f> puntosI2,Mat m_lines );
float distanciaOrtogonal(Point a, Point b, Point c);
float calculaError(Mat &lineas,vector<Point2f> &puntos);
float bondadF(Mat &lineasIm1, Mat &lineasIm2, vector<Point2f> &puntosIm1,vector<Point2f> &puntosIm2);
vector<Mat> proyectaPuntos(vector<Point3f> &puntos3D, Mat &camara);
vector<Point2f> obtenerCoordPixel(vector<Mat> &multiplicados3D);
Mat estimaP( vector<Point3f> puntos3D,vector<Point2f> puntos2D);
double frobeniusErr(Mat matriz);
#endif	/* FUNCIONES_H */

