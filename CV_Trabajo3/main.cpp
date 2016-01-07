/* 
 * File:   main.cpp
 * Author: cris
 *
 * Created on 04 January 2016, 16:34
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

estimaMatrizCamara();

vector<Point3f> puntos1;
vector<Point3f> puntos2;

Point3f p_1;

p_1.x=0; p_1.y=0.1;p_1.z=0.1;
p_1.x=0; p_1.y=0.2;p_1.z=0.1;
p_1.x=0; p_1.y=0.3;p_1.z=0.1;
p_1.x=0; p_1.y=0.4;p_1.z=0.1;
p_1.x=0; p_1.y=0.5;p_1.z=0.1;
p_1.x=0; p_1.y=0.6;p_1.z=0.1;
p_1.x=0; p_1.y=0.7;p_1.z=0.1;
p_1.x=0; p_1.y=0.8;p_1.z=0.1;
p_1.x=0; p_1.y=0.9;p_1.z=0.1;
p_1.x=0; p_1.y=0.10;p_1.z=0.1;




//ejercicio3 
//apartado a)

string criterio="Flann";
//string criterio="BFCrossCheck";

Mat vmort1=leeimagen("imagenes/Vmort1.pgm", -1);
Mat vmort2=leeimagen("imagenes/Vmort2.pgm", -1);
vector<KeyPoint> vmort1_kp;
vector<KeyPoint> vmort2_kp;
Mat descriptor1,descriptor2;
Mat salida_vmort1,salida_vmort2;

aplicaBRISK(vmort1, vmort1_kp,descriptor1,salida_vmort1);
aplicaBRISK(vmort2, vmort2_kp,descriptor2,salida_vmort2);
 
 Mat correspondencias_vmort_brisk;
 vector<DMatch> coincidencias;
 
 correspondencias_vmort_brisk=hallaCorresp(vmort1,vmort2,vmort1_kp,vmort2_kp,descriptor1,descriptor2,criterio,coincidencias);
 imshow("CORRESPONDENCIAS VMORT brisk",correspondencias_vmort_brisk); 
 waitKey(0);
 
//ORB
vector<KeyPoint> vmort1_kp_orb;
vector<KeyPoint> vmort2_kp_orb;
Mat descriptor1_o,descriptor2_o;
Mat salida_vmort1_o,salida_vmort2_o;

aplicaORB(vmort1,vmort1_kp_orb,descriptor1_o,salida_vmort1_o);
aplicaORB(vmort2,vmort2_kp_orb,descriptor2_o,salida_vmort2_o);

 Mat correspondencias_vmort_orb;
 vector<DMatch> coincidencias_orb;
 
 correspondencias_vmort_orb=hallaCorresp(vmort1,vmort2,vmort1_kp_orb,vmort2_kp_orb,descriptor1_o,descriptor2_o,criterio,coincidencias_orb);
 imshow("CORRESPONDENCIAS VMORT orb",correspondencias_vmort_orb); 
 waitKey(0);
 



    return 0;
 
 
 
}

