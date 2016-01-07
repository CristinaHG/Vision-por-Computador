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

Mat hallaCorresp(Mat im1,Mat im2,vector<KeyPoint> kp1,vector<KeyPoint> kp2,Mat descrip1,Mat descrip2,string criterio,vector<DMatch> &coincidencias){

    Mat emparejados;
    
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
   //        cout<<"detrminante original vale :"<<det<<endl;
           
    }while(det<=0.4);

        return MatrizCamara;

}



Mat calculaFundamental( vector<KeyPoint> keypointsIm1, vector<KeyPoint> keypointsIm2, vector<DMatch> coincidencias){

    vector<Point2f> puntosIm1, puntosIm2;
    Mat matrizFundamental;
     
    for(int i=0; i<coincidencias.size(); i++){
        puntosIm1.push_back(keypointsIm1[coincidencias.at(i).queryIdx].pt);
        puntosIm2.push_back(keypointsIm2[coincidencias.at(i).trainIdx].pt);
    }
    
    matrizFundamental=findFundamentalMat(puntosIm1, puntosIm2,FM_RANSAC+CV_FM_8POINT,3.,0.99,noArray() );
    
  return matrizFundamental;  
}