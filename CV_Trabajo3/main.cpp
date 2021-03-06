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
    
//EJERCICIO 1a)
    
 Mat camara;
 camara=estimaMatrizCamara();

 
//EJERCICIO 1b)
 
vector<Point3f> puntos;

Point3f puntoMundo1, puntoMundo2;

//metemos puntos 3D en un vector
    for (double x1 = 0.1; x1 <= 1; x1 += 0.1) {
        for (double x2 = 0.1; x2 <= 1; x2 += 0.1) {
            
            puntoMundo1.x = 0;
            puntoMundo1.y = x1;
            puntoMundo1.z = x2;

            puntos.push_back(puntoMundo1);

            puntoMundo2.x = x2;
            puntoMundo2.y = x1;
            puntoMundo2.z = 0;

            puntos.push_back(puntoMundo2);
        }
    }


//EJERCICIO 1c)

//proyectamos puntos con la camara
vector<Mat> proyectados;
proyectados=proyectaPuntos(puntos,camara);


cout<<"num proyectados="<<proyectados.size()<<endl;

//pasamos de 3x1 a 2x1 (pasamos a pixeles)
vector<Point2f> coorPixel;
coorPixel=obtenerCoordPixel(proyectados);

//imprimimos pixeles
for(int i=0;i<coorPixel.size();i++){

    cout<<"Pixel"<<i<<"= ("<<coorPixel.at(i).x<<","<<coorPixel.at(i).y<<")"<<endl;

}

//EJERCICIO 1d)

//estima P por DLT y la muestra
Mat P;
P=estimaP(puntos,coorPixel);

cout<<" P ="<<P;

//EJERCICIO 1e)

double errorP= frobeniusErr(P);
double errorS=frobeniusErr(camara);
cout<<"ERROR NORMA FROBENIUS"<<cv::abs(errorP-errorS)<<endl;


//EJERCICIO 1f)

muestraPuntos(puntos,P,camara);



//EJERCICIO 2 a y b

String path="/home/cris/mrcrstnherediagmez@gmail.com/VC/CV_Trabajo3/imagenes/chessboard";
vector<String> nombresImagenes;
bool ratio=false;
Mat MCamara;
CheckValidasYcalibra(path,nombresImagenes,ratio,MCamara);

cout<<" intrínseca "<<MCamara<<endl;


//EJERCICIO 3a)

    string criterio = "Flann";
    //string criterio="BFCrossCheck";

    //leemos imagen
    Mat vmort1 = leeimagen("imagenes/Vmort1.pgm", -1);
    Mat vmort2 = leeimagen("imagenes/Vmort2.pgm", -1);

    cout << "TIPO VMORT1" << vmort1.type() << endl;
    cout << "TIPO VMOR2" << vmort2.type() << endl;

    vmort2.convertTo(vmort2, vmort1.type());

    cout << "TIPO VMORT1" << vmort1.type() << endl;
    cout << "TIPO VMOR2" << vmort2.type() << endl;

    vector<KeyPoint> vmort1_kp;
    vector<KeyPoint> vmort2_kp;
    Mat descriptor1, descriptor2;
    Mat salida_vmort1, salida_vmort2;
    
    //aplicamos brisk
    aplicaBRISK(vmort1, vmort1_kp, descriptor1, salida_vmort1);
    aplicaBRISK(vmort2, vmort2_kp, descriptor2, salida_vmort2);

    Mat correspondencias_vmort_brisk;
    vector<DMatch> coincidencias;
    //calculamos corresp tras brisk
    correspondencias_vmort_brisk = hallaCorresp(vmort1, vmort2, vmort1_kp, vmort2_kp, descriptor1, descriptor2, criterio, coincidencias);
    imshow("CORRESPONDENCIAS VMORT brisk", correspondencias_vmort_brisk);
    waitKey(0);

    //ORB
    vector<KeyPoint> vmort1_kp_orb;
    vector<KeyPoint> vmort2_kp_orb;
    Mat descriptor1_o, descriptor2_o;
    Mat salida_vmort1_o, salida_vmort2_o;

    //aplicamos orb
    aplicaORB(vmort1, vmort1_kp_orb, descriptor1_o, salida_vmort1_o);
    aplicaORB(vmort2, vmort2_kp_orb, descriptor2_o, salida_vmort2_o);

    Mat correspondencias_vmort_orb;
    vector<DMatch> coincidencias_orb;
    //calculamos correspondencias tras orb
    correspondencias_vmort_orb = hallaCorresp(vmort1, vmort2, vmort1_kp_orb, vmort2_kp_orb, descriptor1_o, descriptor2_o, criterio, coincidencias_orb);
    imshow("CORRESPONDENCIAS VMORT orb", correspondencias_vmort_orb);
    waitKey(0);



//EJERCICIO 3B)

    vector<Point2f> puntosIm1, puntosIm2;

    Mat F = calculaFundamental(vmort1_kp_orb, vmort2_kp_orb, puntosIm1, puntosIm2, coincidencias_orb);

    cout << F << endl;

    
//EJERCICIO 3C)

    vector<Vec3f> lineasEpip_Im1;
    vector<Vec3f> lineasEpip_Im2;
//calclamos Epilineas
    computeCorrespondEpilines(Mat(puntosIm1), 1, F, lineasEpip_Im1);
    computeCorrespondEpilines(Mat(puntosIm2), 2, F, lineasEpip_Im2);

    Mat lineas1 = Mat(2, lineasEpip_Im1.size(), CV_32F);
    Mat lineas2 = Mat(2, lineasEpip_Im2.size(), CV_32F);

//dibujamos epipolares
    Mat epipolares_im1 = dibujaEpipolares(vmort1, vmort2, lineasEpip_Im1, puntosIm1, puntosIm2, lineas1);
    Mat epipolares_im2 = dibujaEpipolares(vmort2, vmort1, lineasEpip_Im2, puntosIm2, puntosIm1, lineas2);

    imshow("EPIPOLARES IM1", epipolares_im1);
    waitKey(0);
    imshow("EPIPOLARES IM2", epipolares_im2);
    waitKey(0);


//EJERCICIO 3d)
    //calculamos bondad de F
    float error = bondadF(lineas1, lineas2, puntosIm1, puntosIm2);
    cout << " error de  F= " << error << endl;
    return 0;

}

