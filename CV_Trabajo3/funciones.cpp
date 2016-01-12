/* 
 * File:   funciones.cpp
 * Author: cris
 *
 * Created on 04 January 2016, 16:38
 */

#include <vector>
#include "funciones.h"
#include <math.h> 

#include <opencv2/imgproc.hpp>


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
    
    int nfeatures=700;
    float scaleFactor=1.2f;
    int nlevels=10;
    int edgeThreshold=30;
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
     
//    vector<DMatch> mejoresKp;
//    for(int i=0; i<mejoresKp.size();i++){
//        mejoresKp.at(i).distance<
//    }
//    
     
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
    
    Mat MatrizCamara = Mat(3, 4, CV_64F);
    
    double numero;
    float det=0;
    srand (time(NULL));
    
    
    do{
    for(int i=0;i<MatrizCamara.rows;i++){
        for(int j=0; j<MatrizCamara.cols;j++){
            numero = ((double) (rand()) / (double)(RAND_MAX));  
            MatrizCamara.at<double>(i,j)=numero;
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



Mat calculaFundamental( vector<KeyPoint> keypointsIm1, vector<KeyPoint> keypointsIm2, vector<Point2f> &puntosIm1, vector<Point2f> &puntosIm2, vector<DMatch> coincidencias){

    Mat matrizFundamental;//=cv::Mat::zeros(3,3, CV_32F);
     
    for(int i=0; i<coincidencias.size(); i++){
        puntosIm1.push_back(keypointsIm1[coincidencias.at(i).queryIdx].pt);
        puntosIm2.push_back(keypointsIm2[coincidencias.at(i).trainIdx].pt);
    }
    
    matrizFundamental=findFundamentalMat(puntosIm1, puntosIm2,CV_FM_RANSAC+CV_FM_8POINT);
  return matrizFundamental;  
}

Mat dibujaEpipolares(Mat imagen1, Mat imagen2,vector<Vec3f> lineas, vector<Point2f> puntosI1, vector<Point2f> puntosI2 ,Mat m_lines){

    int ncol=imagen1.cols;
    //int nfilas=imagen1.rows;
    int lineasPintadas=0;
    Point punto1,punto2;
    Scalar color;
    RNG random;
    
    cv::cvtColor(imagen1,imagen1,cv::COLOR_GRAY2BGR);
    cv::cvtColor(imagen2,imagen2,cv::COLOR_GRAY2BGR);
    
    cout <<" TIPO IM1"<<imagen1.type()<<endl;

    for (int i=0;i<lineas.size(); i++){
			
		if (lineasPintadas <200){                   
                        color=Scalar( random.uniform(0,257), random.uniform(0,257), random.uniform(0,257));
			punto1 = Point(0, -lineas.at(i)[2] / lineas.at(i)[1]);
			punto2 = Point(ncol,-(lineas.at(i)[2] + lineas.at(i)[0] * ncol) / lineas.at(i)[1]);
			line(imagen1, punto1, punto2,color);
			m_lines.at<Point>(0, lineasPintadas) = punto1;
			m_lines.at<Point>(1, lineasPintadas) = punto2;
			lineasPintadas++;
                        
		}
	}
    return imagen1;
}

float distanciaOrtogonal(Point a, Point b, Point c){

    float distancia;
//    Point numerador=b-a;
//    Point denominador=c-a;
//    float area= c.cross(numerador);
//    distancia=area/norm(numerador);
//    
    distancia=((c.cross(b-a))/norm(b-a));
    return distancia;
}



float calculaError(Mat &lineas,vector<Point2f> &puntos){
    float error=0;
    float distancia;
    
    for(int i=0;i<lineas.rows;i++){
        distancia=distanciaOrtogonal(lineas.at<Point>(0,i),lineas.at<Point>(1,i),puntos.at(i));
        error= error+distancia;
        error=error/lineas.rows;
    }
    return error;
}


float bondadF(Mat &lineasIm1, Mat &lineasIm2, vector<Point2f> &puntosIm1,
		vector<Point2f> &puntosIm2)
{
	float errorIm1= 0;
	float errorIm2=0;
        float error;
	errorIm1=calculaError(lineasIm1,puntosIm1);
        errorIm2=calculaError(lineasIm2,puntosIm2);
        error=(errorIm1+errorIm2)/2;
	return error;
}


vector<Mat> proyectaPuntos(vector<Point3f> &puntos3D, Mat &camara){

    vector<Mat> puntos4f;
    vector<Mat> proyectados;
    
    for(int i=0;i<puntos3D.size();i++){       
         Mat m= cv::Mat(4,1, CV_64F);
         m.at<double>(0,0)=puntos3D.at(i).x;
         m.at<double>(1,0)=puntos3D.at(i).y;
         m.at<double>(2,0)=puntos3D.at(i).z;
         m.at<double>(3,0)=1;
         puntos4f.push_back(m);
    }
  
//     for(int i=0;i<puntos4f.size();i++){
//      cout<<puntos4f.at(i);
//    }

    

    for(int i=0; i< puntos4f.size();i++){
        Mat multiplicacion;
        multiplicacion=camara*puntos4f.at(i);
        proyectados.push_back(multiplicacion);
    } 
    
    return proyectados;
}

vector<Point2f> obtenerCoordPixel(vector<Mat> &multiplicados3D){
    
    vector<Point2f> pixeles;
    
    //cout<<"PRIMERO DE MULTIPLICADOS"<<multiplicados3D.at(0).at<float>(0,0);
    
    for(int i=0;i < multiplicados3D.size();i++){
        Point2f pixel;
        
        pixel.x=multiplicados3D.at(i).at<double>(0,0)/multiplicados3D.at(i).at<double>(2,0);
        pixel.y=multiplicados3D.at(i).at<double>(1,0)/multiplicados3D.at(i).at<double>(2,0);
        
        pixeles.push_back(pixel);   
    }
    return pixeles;
}

Mat estimaP( vector<Point3f> &puntos3D,vector<Point2f> &puntos2D){

    const int n_puntosMundo=puntos3D.size();
     Mat A(2*n_puntosMundo, 12, CV_64F);     
     int j=0;
     int row_act=0;

     for(int i=0; i<n_puntosMundo;i++){ //para cada uno de los puntos
         //obtenemos 1º fila de Ai
        
            A.at<double>(row_act,j)=puntos3D.at(i).x;j+=1;         
            A.at<double>(row_act,j)=puntos3D.at(i).y;j+=1; 
            A.at<double>(row_act,j)=puntos3D.at(i).z;j+=1; 
            A.at<double>(row_act,j)=1;j+=1;
            A.at<double>(row_act,j)=0;j+=1;
            A.at<double>(row_act,j)=0;j+=1;
            A.at<double>(row_act,j)=0;j+=1;
            A.at<double>(row_act,j)=0;j+=1;
            A.at<double>(row_act,j)=-(puntos2D.at(i).x * puntos3D.at(i).x);j+=1;
            A.at<double>(row_act,j)=-(puntos2D.at(i).x * puntos3D.at(i).y);j+=1;
            A.at<double>(row_act,j)=-(puntos2D.at(i).x * puntos3D.at(i).z);j+=1;
            A.at<double>(row_act,j)=-puntos2D.at(i).x;

             //obtenemos 2º fila de Ai
            
         row_act+=1;
         j=0;
         
            A.at<double>(row_act,j)=0;j+=1;         
            A.at<double>(row_act,j)=0;j+=1; 
            A.at<double>(row_act,j)=0;j+=1; 
            A.at<double>(row_act,j)=0;j+=1; 
            A.at<double>(row_act,j)=-puntos3D.at(i).x;j+=1;
            A.at<double>(row_act,j)=-puntos3D.at(i).y;j+=1;
            A.at<double>(row_act,j)=-puntos3D.at(i).z;j+=1;
            A.at<double>(row_act,j)=-1;j+=1;
            A.at<double>(row_act,j)=(puntos2D.at(i).y *puntos3D.at(i).x);j+=1;
            A.at<double>(row_act,j)=(puntos2D.at(i).y *puntos3D.at(i).y);j+=1;
            A.at<float>(row_act,j)=(puntos2D.at(i).y *puntos3D.at(i).z);j+=1;
            A.at<float>(row_act,j)=puntos2D.at(i).y;
            
            row_act+=1;
            //reseteamos j para la siguiente interacción
            j=0; 
                       
     }

       cv::SVD svd(A, cv::SVD::MODIFY_A);
       return svd.vt.row(11).reshape(0, 3);

}


double frobeniusErr(Mat matriz){
    double normaFrobenius;
//    double suma_col=0;
//    double suma_f=0;
//    
//    for(int i=0;i<matriz.rows;i++){
//        for(int j=0;j<matriz.cols;j++){
//           suma_col=suma_col+(matriz.at<double>(i,j)*matriz.at<double>(i,j));   
//        }
//        suma_f+=suma_col;
//    }
//    normaFrobenius=cv::sqrt(suma_f);
    normaFrobenius=cv::norm(matriz,NORM_L2);
    return normaFrobenius;
    
}

void muestraPuntos(vector<Point3f> puntos3D, Mat &estimada, Mat &simulada){

    Mat imagen(500, 1000, CV_8UC3, Scalar(0, 0, 0));
    
    vector<Mat> proyectados_E=proyectaPuntos(puntos3D, estimada);
    vector<Mat> proyectados_S=proyectaPuntos(puntos3D, simulada);
  
    
    vector<Point2f> pixels_S=obtenerCoordPixel(proyectados_S);
    vector<Point2f> pixels_E=obtenerCoordPixel(proyectados_E);
    
 
     for(int i=0; i<pixels_S.size();i++){
         pixels_S.at(i).x*=100;
         pixels_S.at(i).y*=100;
         pixels_E.at(i).x*=100;
         pixels_E.at(i).y*=100;    
    }
    
    cout<<pixels_E;
//    cout<<"S size"<< pixels_S.size()<<endl;
//      cout<<"E size"<< pixels_E.size()<<endl;
    for(int i=0; i<pixels_S.size();i++){
         imagen.at<Vec3b>(pixels_S.at(i).x, pixels_S.at(i).y) =Vec3b(255,255,0);
         imagen.at<Vec3b>(pixels_E.at(i).x, pixels_E.at(i).y) =Vec3b(255,0,255);    
    }
    
    //namedWindow("puntos Estimada & Simulada", WINDOW_AUTOSIZE);
    imshow("puntos Estimada & Simulada", imagen);
    waitKey(0);
    destroyWindow("puntos Estimada & Simulada");
}

void CheckValidas(String ruta, vector<String> nombresIm){
    
    vector<Mat> imgValidas; 
    vector<vector<Point2d> > esquinas;

    cv::Size tam=Size(13,12);
    
    Mat imag_gris;
         vector<Point2f> corners;  
    cv::glob(ruta,nombresIm);
//    cout<<nombresIm.size();
    
    for(int i=0;i<nombresIm.size();i++){
        Mat imagen=imread(nombresIm.at(i));

       // waitKey(0);
 
        bool valida=cv::findChessboardCorners(imagen,tam,corners,CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        cv::cvtColor(imagen,imag_gris,CV_BGR2GRAY);
//        cout<<"NUM CANALES IM "<<imagen.channels()<<endl;
//        cout<<"NUM CANALES GRIS "<<imag_gris.channels()<<endl;
        if(valida){
        imgValidas.push_back(imag_gris);   
        cv::cornerSubPix(imag_gris,corners,Size(11, 11),Size(-1, -1),TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
         cv::cvtColor(imag_gris,imag_gris,CV_GRAY2RGB);
        drawChessboardCorners(imag_gris,tam,Mat(corners),valida); 
        
           imshow("chessboard corners",imag_gris);
           waitKey(0);
        }

        
        
        
    }
cout<<"NUM VALIDAS= "<<imgValidas.size();
}

