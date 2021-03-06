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

Mat calculaMosaico(Mat im1, Mat im2,vector<KeyPoint> keypoints1,vector<KeyPoint> keypoints2,vector<DMatch> coincidencias){
    Mat mosaico;
    Mat homografia1, homografia2;
    vector<Point2f> puntosIm1, puntosIm2;

  //   homografia1=Mat(3,3, CV_32F);
     
    for(int i=0; i<coincidencias.size(); i++){
        puntosIm1.push_back(keypoints1[coincidencias.at(i).queryIdx].pt);
        puntosIm2.push_back(keypoints2[coincidencias.at(i).trainIdx].pt);
    }
    
//    for(int i=0; i<keypoints2.size(); i++){
//        puntosIm2.at(i).x=keypoints2.at(i).pt.x;
//        puntosIm2.at(i).y=keypoints2.at(i).pt.y;        
//    }

   homografia1= cv::Mat::zeros(3,3, CV_32F);
   
	homografia1.at<float>(0, 0) = 1;
	homografia1.at<float>(0, 2) = floor(im2.rows/2);
	homografia1.at<float>(1, 1) = 1;
	homografia1.at<float>(1, 2) = floor(im2.cols/2);
	homografia1.at<float>(2, 2) = 1;  
    
        cout<<"HOMOGRAFIA 1 BIEN"<<homografia1<<endl;
    homografia2=findHomography(puntosIm1, puntosIm2, CV_RANSAC, 1);
    cout<<"HOMOGRAFIA 2 BIEN"<<homografia2<<endl;
    Size tam_mosaico;
    tam_mosaico.height=im1.rows+im2.rows;
    tam_mosaico.width=im2.cols+im2.cols;
    
    warpPerspective(im2, mosaico, homografia1, tam_mosaico);
    homografia1.convertTo(homografia1,homografia2.type());
    cout << "TIPO HOMO1"<<homografia1.type()<<endl;
    cout << "TIPO HOMO2"<<homografia2.type()<<endl;
    homografia2=homografia1*homografia2;
    warpPerspective(im1,mosaico, homografia2,tam_mosaico,  INTER_LINEAR, BORDER_TRANSPARENT);
    return mosaico;
}

Mat calculaMosaicoMultiples(vector<Mat> imagenes ,vector<vector<KeyPoint> > keypoints,vector<vector<DMatch> > coincidencias){
     Mat mosaico;
    Mat homografia1, homografia2;
    vector<vector<Point2f> > puntosIm;
    cout<<"COINCIDENCIAS1.SIZE "<<coincidencias.at(0).size()<<endl;
cout<<"COINCIDENCIAS2.SIZE "<<coincidencias.at(1).size()<<endl;
  //   homografia1=Mat(3,3, CV_32F);
    
    for(int i=0; i<coincidencias.size(); i++){
        vector<Point2f> pI1,pI2;
        vector<DMatch> match; 
        //match.
        //match.push_back(coincidencias.at(i));
        for(int j=0;j<coincidencias.at(i).size();j++){          
            pI1.push_back(keypoints.at(i)[coincidencias.at(i).at(j).queryIdx].pt);            
            pI2.push_back(keypoints.at(i+1)[coincidencias.at(i).at(j).trainIdx].pt);           
        }
        puntosIm.push_back(pI1);
        puntosIm.push_back(pI2);
    }

    homografia1= cv::Mat::zeros(3,3, CV_32F);
    
	homografia1.at<float>(0, 0) = 1;
	homografia1.at<float>(0, 2) = floor(imagenes.at(0).rows/3);
	homografia1.at<float>(1, 1) = 1;
	homografia1.at<float>(1, 2) = floor(imagenes.at(0).cols/3);
	homografia1.at<float>(2, 2) = 1;  
    
    cout<<"HOMOGRAFIA 1 MAL"<<homografia1<<endl;
    int ancho=0;
    for(int i=0;i<imagenes.size();i++){
        ancho+=imagenes.at(i).cols;
    }
    
    Size tam_mosaico;
    tam_mosaico.height=2*imagenes.at(0).rows;
    tam_mosaico.width=ancho;
    
//    cv::imshow("PRIMERA IMAGEN",imagenes.at(0));
//    waitKey(0);
    warpPerspective(imagenes.at(1), mosaico, homografia1, tam_mosaico);
    
    homografia2=findHomography(puntosIm.at(0), puntosIm.at(1), CV_RANSAC, 1);
    cout<<"HOMOGRAFIA 2 MAL"<<homografia2<<endl;
    cout << "TIPO HOMO1"<<homografia1.type()<<endl;
    cout << "TIPO HOMO2"<<homografia2.type()<<endl;
    homografia1.convertTo(homografia1,homografia2.type());
    cout << "TIPO HOMO1"<<homografia1.type()<<endl;
    cout << "TIPO HOMO2"<<homografia2.type()<<endl;
    homografia2=homografia1*homografia2;
    warpPerspective(imagenes.at(0),mosaico, homografia2,tam_mosaico,  INTER_LINEAR, BORDER_TRANSPARENT);
    
    
    Mat homografiaNueva=findHomography(puntosIm.at(2), puntosIm.at(3), CV_RANSAC,1);
    homografiaNueva=homografiaNueva*homografia2;
    warpPerspective(imagenes.at(2),mosaico, homografiaNueva,tam_mosaico,  INTER_LINEAR, BORDER_TRANSPARENT);
    
//    
//    warpPerspective(im2, mosaico, homografia1, tam_mosaico);
//    
//    for(int i=1;i<imagenes.size();i++){
//    
//    
//    }
//    homografia1.convertTo(homografia1,homografia2.type());
//    cout << "TIPO HOMO1"<<homografia1.type()<<endl;
//    cout << "TIPO HOMO2"<<homografia2.type()<<endl;
//    homografia2=homografia1*homografia2;
//    warpPerspective(im1,mosaico, homografia2,tam_mosaico,  INTER_LINEAR, BORDER_TRANSPARENT);
//    
    
    return mosaico;      
}