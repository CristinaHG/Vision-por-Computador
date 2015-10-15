
#include <cstdlib>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "funciones.h"

Mat calculaMascara(float sigma) {

    //int intervalo=3*sigma;
    float intervalo = sigma;
    float normalizacion;
    vector<float> inter;

    
        for(int i=-intervalo;i<=intervalo;i++){
            inter.push_back((float)i);  
        }

    Mat mascara(1, inter.size(), DataType<float>::type);

    for (int i = 0; i < inter.size(); i++) {
        mascara.at<float>(0, i) = inter.at(i);
    }

    float suma=0;
    for (int i = 0; i < mascara.cols; i++) {
        mascara.at<float>(0, i) =(float) exp(-0.5 * ((mascara.at<float>(0, i) * mascara.at<float>(0, i)) / (sigma * sigma)));
        suma+=mascara.at<float>(0, i);
    }
     
   //normalizacion
    
 //   normalizacion = 1.0 / (mascara.cols);
    
    for (int i = 0; i < mascara.cols; i++) {
        mascara.at<float>(0, i) = (mascara.at<float>(0, i) *(1/suma));
    }
 
    //comprobar que la máscara suma 1

    cout << "suma mascara " << sum(mascara) << endl;

    return mascara;

}
