/* 
 * File:   funciones.cpp
 * Author: cris
 *
 * Created on 04 January 2016, 16:38
 */

#include <vector>

#include "funciones.h"
#include <math.h>  

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

//int calculaDeterminante(Mat matriz){
//
//    int j=0;
//    int det;
//    Mat menorDeOrden3;
//
//    if(matriz.cols>3 && matriz.rows>3 ){
//    
//        for(int i=0; i<matriz.cols && det==0;i++){
//            menorDeOrden3= matriz(Rect(i,j,3,3));
//            det=determinant(menorDeOrden3);
//        } 
//    }
//return det;
//}


Mat estimaMatrizCamara(){
    
    Mat MatrizCamara = Mat(3, 4, CV_32F);
    
    float numero;
    float det=0;
    srand (time(NULL));
    
    
    do{
    for(int i=0;i<MatrizCamara.rows;i++){
        for(int j=0; j<MatrizCamara.cols;j++){
            numero = ((float) (rand()) / (float)(RAND_MAX));  
            MatrizCamara.at<float>(i,j)=numero;
        }
    }
    
//        for(int i=0;i<MatrizCamara.rows;i++){
//            for(int j=0; j<MatrizCamara.cols;j++){
//                cout<<MatrizCamara.at<float>(i,j)<<" ";
//            }
//            cout<<endl;
//        }
    
    Mat menorDeOrden3= MatrizCamara(Rect(0,0,3,3));
    det=determinant(menorDeOrden3);
//           cout<<"detrminante original vale :"<<det<<endl;
           
    }while(det<=0.3);

        return MatrizCamara;

}