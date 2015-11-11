/* 
 * File:   main.cpp
 * Author: cris
 *
 * Created on 9 November 2015, 22:20
 */


#include <vector>

#include "funciones.h"

Mat leeimagen(string filename, int flagColor) {
    Mat im;
    if (flagColor < 0) //lee imagen tal como esta es
        im = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
    else if (flagColor == 0) //lee imágen en blanco y negro
        im = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    else if (flagColor > 0) //lee imágen en color en formato RGB
        im = imread(filename, CV_LOAD_IMAGE_COLOR);
    return im;
}

void pintaI(string im) {
    namedWindow("pinta Imagen", WINDOW_AUTOSIZE);
    imshow("pinta Imagen", leeimagen(im, -1));
    waitKey(0);
    destroyWindow("pinta Imagen");
}

Mat EstimaHomografia( Mat im1, vector<Point2f> puntosI1,vector<Point2f> puntosI2){
    
     const int n_puntos=puntosI1.size();
     Mat A(2*n_puntos, 9, CV_32F);     
     int j=0;
     int row_act=0;
     
     for(int i=0; i<n_puntos;i++){ //para cada uno de los puntos
         //obtenemos 1º fila de Ai
        
            A.at<float>(row_act,j)=-puntosI1.at(i).x;j+=1;         
            A.at<float>(row_act,j)=-puntosI1.at(i).y;j+=1; 
            A.at<float>(row_act,j)=-1;j+=1; 
            A.at<float>(row_act,j)=0;j+=1;
            A.at<float>(row_act,j)=0;j+=1;
            A.at<float>(row_act,j)=0;j+=1;
            A.at<float>(row_act,j)=(puntosI1.at(i).x * puntosI2.at(i).x);j+=1;
            A.at<float>(row_act,j)=(puntosI1.at(i).y *puntosI2.at(i).x);j+=1;
            A.at<float>(row_act,j)=puntosI2.at(i).x;

         //obtenemos 2º fila de Ai
            
         row_act+=1;
         j=0;
         
            A.at<float>(row_act,j)=0;j+=1;         
            A.at<float>(row_act,j)=0;j+=1; 
            A.at<float>(row_act,j)=0;j+=1; 
            A.at<float>(row_act,j)=-puntosI1.at(i).x;j+=1;
            A.at<float>(row_act,j)=-puntosI1.at(i).y;j+=1;
            A.at<float>(row_act,j)=-1;j+=1;
            A.at<float>(row_act,j)=(puntosI1.at(i).x *puntosI2.at(i).y);j+=1;
            A.at<float>(row_act,j)=(puntosI1.at(i).y *puntosI2.at(i).y);j+=1;
            A.at<float>(row_act,j)=puntosI2.at(i).y;
            
            row_act+=1;
            //reseteamos j para la siguiente interacción
            j=0;                 
     }
     //cout<<A<<endl;
       cv::SVD svd(A, cv::SVD::MODIFY_A | cv::SVD::FULL_UV);
       //cout<< svd.vt.row(8).reshape(0, 3)<<endl;
       return svd.vt.row(8).reshape(0, 3);
}
