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

//////////////////////////////////////funciones de prácticas anteriores ///////////////////////
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
        
        cv::FlannBasedMatcher flann(new cv::flann::LshIndexParams(15,15,0));
        flann.match(descrip1, descrip2,coincidencias);
    } 
     
    drawMatches(im1,kp1,im2,kp2,coincidencias,emparejados);
    
    return emparejados;
}





//--------------------------------FUNCIONES EJERCICIO 1-----------------------------


Mat estimaMatrizCamara(){ //función que estima una cámara con valores aleatorios[0-1]
    
    Mat MatrizCamara = Mat(3, 4, CV_64F);
    
    double numero;
    float det=0;
    
    srand (time(NULL));//inicializar semilla
    
    
    do{ //dowhile externo que repetirá el proceso hasta que el det de la matriz sea >0.4
        
    for(int i=0;i<MatrizCamara.rows;i++){ //for internos que rellenan la matriz camara
        for(int j=0; j<MatrizCamara.cols;j++){
            numero = ((double) (rand()) / (double)(RAND_MAX)); 
            MatrizCamara.at<double>(i,j)=numero;
        }
    }
    
    Mat menorDeOrden3= MatrizCamara(Rect(0,0,3,3));//como e 3x4, tomamos menor 3x3
    det=determinant(menorDeOrden3); //hacemos determinante
           
    }while(det<=0.4);

        return MatrizCamara;

}


//función qe proyectas los puntos del mundo multiplicando por la cámara
vector<Mat> proyectaPuntos(vector<Point3f> &puntos3D, Mat &camara){

    vector<Mat> puntos4f;
    vector<Mat> proyectados;
    
    //pasamos puntos de 3x1 a 4x1
    for(int i=0;i<puntos3D.size();i++){       
         Mat m= cv::Mat(4,1, CV_64F);
         m.at<double>(0,0)=puntos3D.at(i).x;
         m.at<double>(1,0)=puntos3D.at(i).y;
         m.at<double>(2,0)=puntos3D.at(i).z;
         m.at<double>(3,0)=1;
         puntos4f.push_back(m);
    }

    //multiplicamos por la cámara
    for(int i=0; i< puntos4f.size();i++){
        Mat multiplicacion;
        multiplicacion=camara*puntos4f.at(i);
        proyectados.push_back(multiplicacion);
    } 
    //devuelve los puntos proyectados
    return proyectados;
}

vector<Point2f> obtenerCoordPixel(vector<Mat> &multiplicados3D){
    
    vector<Point2f> pixeles;
    
    //pasamos puntos 3D a pixeles (puntos 2D)
    //diviendo x e y por la coordenada z
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
     //calculamos autovalores y nos quedamos con el más chico
       cv::SVD svd(A, cv::SVD::MODIFY_A);
       return svd.vt.row(11).reshape(0, 3);

}


double frobeniusErr(Mat matriz){
    //calcula el error de frobenius
    double normaFrobenius;
    double suma_col=0;
    double suma_f=0;
    
    for(int i=0;i<matriz.rows;i++){
        for(int j=0;j<matriz.cols;j++){
           suma_col=suma_col+(matriz.at<double>(i,j)*matriz.at<double>(i,j));   
        }
        suma_f+=suma_col;
    }
    return normaFrobenius;   
}

void muestraPuntos(vector<Point3f> puntos3D, Mat &estimada, Mat &simulada){
    //declaramos canvas grande
    Mat imagen(500, 1000, CV_8UC3, Scalar(0, 0, 0));
    //proyectamos los puntos con las dos matrices camaras
    vector<Mat> proyectados_E=proyectaPuntos(puntos3D, estimada);
    vector<Mat> proyectados_S=proyectaPuntos(puntos3D, simulada);
  
    //pasamos a coordenadas pixel
    vector<Point2f> pixels_S=obtenerCoordPixel(proyectados_S);
    vector<Point2f> pixels_E=obtenerCoordPixel(proyectados_E);
    
    //para poder dibujar, multiplicamos por 100 los puntos
    for(int i=0; i<pixels_S.size();i++){
         pixels_S.at(i).x*=100;
         pixels_S.at(i).y*=100;
         pixels_E.at(i).x*=100;
         pixels_E.at(i).y*=100;    
    }
    
//    cout<<pixels_E;
    
    //pintamos pixeles
    for(int i=0; i<pixels_S.size();i++){
         imagen.at<Vec3b>(pixels_S.at(i).x, pixels_S.at(i).y) =Vec3b(255,255,0);
         imagen.at<Vec3b>(pixels_E.at(i).x, pixels_E.at(i).y) =Vec3b(255,0,255);    
    }
    
    imshow("puntos Estimada & Simulada", imagen);
    waitKey(0);
    destroyWindow("puntos Estimada & Simulada");
}

//-------------------------FUNCIONES IMPLEMENTADAS PARA EL EJERCICIO 2------------------------------

//comprueba que imágenes en una ruta dada son válidas para caibrrar una cámara
//y la calibra
void CheckValidasYcalibra(String ruta, vector<String> nombresIm, bool ratio,Mat &cameraMatrix){
    
    vector<Mat> imgValidas; 
    vector<vector<Point2f> > PuntosEsquinas;

    cv::Size tam=Size(13,12);//queremos que reconozca el máximo del tablero
    
    Mat imag_gris;
    vector<Point2f> corners;//vector que contendrá las esquinas que vaya encontrando 
    
    cv::glob(ruta,nombresIm);//leemos imagenes

    
    for(int i=0;i<nombresIm.size();i++){//para cada imágen, buscamos esquinas en el tablero
        
        Mat imagen=imread(nombresIm.at(i));
        bool valida=cv::findChessboardCorners(imagen,tam,corners,CALIB_CB_ADAPTIVE_THRESH |
        CV_CALIB_CB_FILTER_QUADS);
        
        if(valida){ //si la imágen vale para calibrar
            
            cv::cvtColor(imagen,imag_gris,CV_BGR2GRAY);
            imgValidas.push_back(imagen);
            cv::cornerSubPix(imag_gris,corners,Size(11, 11),Size(-1, -1),TermCriteria(CV_TERMCRIT_EPS +
            CV_TERMCRIT_ITER, 30, 0.1));//buscamos esquinas con más precisión
            
            PuntosEsquinas.push_back(corners);
            
            cv::cvtColor(imag_gris,imag_gris,CV_GRAY2RGB);
            drawChessboardCorners(imag_gris,tam,Mat(corners),valida);//muestra imagen 
        
            imshow("chessboard corners",imag_gris);
            waitKey(0);
            
        }        
    }
    
    vector<vector<Point3f> > objPoints;
    
   //obtenemos objetos
    for( int i = 0; i < PuntosEsquinas.size();i++ ){
        vector<Point3f> puntos3D;
        for( int j = 0; j < tam.height;j++ ){
            for(int t=0; t< tam.width;t++){
               puntos3D.push_back(Point3f(float(5*t), float(5*j), 0));    
            }
        }
        objPoints.push_back(puntos3D);
    }
    //inicializamos la matriz camara
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    //declaramos coefDistorsion y los vectores de rot y transl
    Mat distCoeffs = Mat::zeros(8, 1, CV_64F);
    vector<Mat> v_rot;
    vector<Mat> v_trans;
    //se hace la calibración
    double sol=cv::calibrateCamera(objPoints,PuntosEsquinas,imgValidas.front().size(),cameraMatrix,distCoeffs,v_rot,v_trans,
           CV_CALIB_FIX_K4 |CV_CALIB_FIX_K5 );
    
cout<<"ERR "<<sol;

}


//-------------------------------FUNCIONES IMPLEMENTADAS PARA EJERCICIO 3--------------------------

Mat calculaFundamental( vector<KeyPoint> keypointsIm1, vector<KeyPoint> keypointsIm2, vector<Point2f> &puntosIm1, vector<Point2f> &puntosIm2, vector<DMatch> coincidencias){

    Mat matrizFundamental;
    //separa los puntos de son de la imagen1 y los puntos que son de la imagen2
    for(int i=0; i<coincidencias.size(); i++){
        puntosIm1.push_back(keypointsIm1[coincidencias.at(i).queryIdx].pt);
        puntosIm2.push_back(keypointsIm2[coincidencias.at(i).trainIdx].pt);
    }
    //calcula matriz Fundamental por ransac + 8 puntos
   matrizFundamental=findFundamentalMat(puntosIm1, puntosIm2,CV_FM_RANSAC+CV_FM_8POINT);
   
  return matrizFundamental;
  
}

Mat dibujaEpipolares(Mat imagen1, Mat imagen2,vector<Vec3f> lineas, vector<Point2f> puntosI1, vector<Point2f> puntosI2 ,Mat m_lines){

    int ncol=imagen1.cols;
    //int nfilas=imagen1.rows;
    int lineasPintadas=0; //llevamos control de las lineas pintadas
    Point punto1,punto2;
    Scalar color;
    
    RNG random; //para pintar las lineas de colores
    
    //pasamos a gris
    cv::cvtColor(imagen1,imagen1,cv::COLOR_GRAY2BGR);
    cv::cvtColor(imagen2,imagen2,cv::COLOR_GRAY2BGR);
    
    //pintamos lineas epipolares hasta llegar a 200
    for (int i=0;i<lineas.size(); i++){
			
		if (lineasPintadas <200){ 
                        //ponemos un color aleatorio
                        color=Scalar( random.uniform(0,257), random.uniform(0,257), random.uniform(0,257));
                        //calculamos puntos epipolares
			punto1 = Point(0, -lineas.at(i)[2] / lineas.at(i)[1]);
			punto2 = Point(ncol,-(lineas.at(i)[2] + lineas.at(i)[0] * ncol) / lineas.at(i)[1]);
                        //dibujamos la linea
			line(imagen1, punto1, punto2,color);
                        //guardamos valores en la matriz
			m_lines.at<Point>(0, lineasPintadas) = punto1;
			m_lines.at<Point>(1, lineasPintadas) = punto2;
                        //incrementamos contador
			lineasPintadas++;
                        
		}
	}
    return imagen1;
}

float distanciaOrtogonal(Point a, Point b, Point c){
  //calcula distancia de dos puntos a otro por el que
  //pasa una recta
    float distancia=((c.cross(b-a))/norm(b-a));
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
    //calcula la media de los errores
	float errorIm1= 0;
	float errorIm2=0;
        float error;
	errorIm1=calculaError(lineasIm1,puntosIm1);
        errorIm2=calculaError(lineasIm2,puntosIm2);
        error=(errorIm1+errorIm2)/2;
	return error;
}

