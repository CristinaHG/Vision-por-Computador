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

Mat estimaHomografia( Mat im1, vector<Point2f> puntosI1,vector<Point2f> puntosI2){
    
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

Mat aplicaBRISK(Mat original, vector<KeyPoint> &keypoints,Mat &descriptor, Mat salida){
    int thresh=65; //calcula el de arriba, los otros no
    int octaves=5;
    float patternScales=1.5f;
    
 
    Ptr<BRISK> detector =BRISK::create(thresh,octaves,patternScales);
    
    detector->detect(original,keypoints);
    detector->compute(original,keypoints,descriptor);

    drawKeypoints(original,keypoints,salida);
    return salida;
}

Mat aplicaORB(Mat original, vector<KeyPoint> &keypoints,Mat &descriptor, Mat salida){
    
    int nfeatures=750;
    float scaleFactor=1.3f;
    int nlevels=9;
    int edgeThreshold=31;
    int firstLevel=0;
    int WTA_K=3;
    int scoreType=ORB::HARRIS_SCORE;
    int patchSize=31;
    
    Ptr<ORB> detector =ORB::create(nfeatures,scaleFactor,nlevels,edgeThreshold,firstLevel,WTA_K);
    
    detector->detect(original,keypoints);
    detector->compute(original,keypoints,descriptor);

    drawKeypoints(original,keypoints,salida);
    return salida;
}

Mat hallaCorresp(Mat im1,Mat im2,vector<KeyPoint> kp1,vector<KeyPoint> kp2,Mat descrip1,Mat descrip2,string criterio){

    Mat emparejados;
    vector<DMatch> coincidencias;
     coincidencias.clear();   
    if(criterio.compare("BFCrossCheck")==0){

        bool crossCheck;
        
        BFMatcher m(NORM_HAMMING, crossCheck=true); 
        m.match(descrip1, descrip2,coincidencias);
        
    }else if(criterio.compare("Flann")==0){
 //       Ptr<DescriptorMatcher> flann=DescriptorMatcher::create("FlannBased");
 //       flann->match(descrip1, descrip2,coincidencias);
        
        cv::FlannBasedMatcher flann(new cv::flann::LshIndexParams(15,15,0));
        flann.match(descrip1, descrip2,coincidencias);
    } 
    drawMatches(im1,kp1,im2,kp2,coincidencias,emparejados);
    
    return emparejados;
}