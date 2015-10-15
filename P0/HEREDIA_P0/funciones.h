/* 
 * File:   funciones.h
 * Author: cris
 *
 * Created on 05 October 2015, 12:07
 */
using namespace cv;
using namespace std; 

#ifndef FUNCIONES_H
#define	FUNCIONES_H

//ejercicio1:
Mat leeimagen(string filename, int flagColor);
//ejercicio2:
void pintaI(string im);

//ejercicio3:
void pintaMI(vector<string> vim);

//ejercicio4
void modificaPixel(string im, vector<Point> pt);
#endif	/* FUNCIONES_H */

