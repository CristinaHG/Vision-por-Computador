/* 
 * File:   main.cpp
 * Author: cris
 *
 * Created on 9 November 2015, 22:20
 */

#include <cstdlib>

#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>

#include "funciones.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {

vector<Point2f> puntos1;
vector<Point2f> puntos2;

puntos1.clear(); 
puntos2.clear();
Point2f i1_1,i1_2,i1_3,i1_4,i1_5,i1_6,i1_7,i1_8,i1_9,i1_10;
Point2f i2_1,i2_2,i2_3,i2_4,i2_5,i2_6,i2_7,i2_8,i2_9,i2_10;

//tomando puntos distribuidos por el tablero 1
//i1_1.x=154; i1_1.y=72; puntos1.push_back(i1_1);
//i2_1.x=141; i2_1.y=40; puntos2.push_back(i2_1);
//i1_2.x=289; i1_2.y=33; puntos1.push_back(i1_2);
//i2_2.x=295; i2_2.y=46; puntos2.push_back(i2_2);
//i1_3.x=530; i1_3.y=11; puntos1.push_back(i1_3);
//i2_3.x=502; i2_3.y=95; puntos2.push_back(i2_3);
//i1_4.x=282; i1_4.y=219; puntos1.push_back(i1_4);
//i2_4.x=260; i2_4.y=219; puntos2.push_back(i2_4);
//i1_5.x=471; i1_5.y=247; puntos1.push_back(i1_5);
//i2_5.x=422; i2_5.y=274; puntos2.push_back(i2_5);
//i1_6.x=471; i1_6.y=247; puntos1.push_back(i1_6);
//i2_6.x=422; i2_6.y=274; puntos2.push_back(i2_6);
//i1_7.x=185; i1_7.y=296; puntos1.push_back(i1_7);
//i2_7.x=147; i2_7.y=274; puntos2.push_back(i2_7);
//i1_8.x=325; i1_8.y=388; puntos1.push_back(i1_8);
//i2_8.x=279; i2_8.y=372; puntos2.push_back(i2_8);
//i1_9.x=158; i1_9.y=398; puntos1.push_back(i1_9);
//i2_9.x=104; i2_9.y=366; puntos2.push_back(i2_9);
//i1_10.x=467; i1_10.y=427; puntos1.push_back(i1_10);
//i2_10.x=392; i2_10.y=413; puntos2.push_back(i2_10);

//tomando puntos en tres cuadrados contiguos
i1_1.x=156; i1_1.y=47; puntos1.push_back(i1_1);
i2_1.x=147; i2_1.y=13; puntos2.push_back(i2_1);
i1_2.x=176; i1_2.y=45; puntos1.push_back(i1_2);
i2_2.x=173; i2_2.y=19; puntos2.push_back(i2_2);
i1_3.x=154; i1_3.y=72; puntos1.push_back(i1_3);
i2_3.x=141; i2_3.y=40; puntos2.push_back(i2_3);
i1_4.x=176; i1_4.y=71; puntos1.push_back(i1_4);
i2_4.x=168; i2_4.y=46; puntos2.push_back(i2_4);
i1_5.x=153; i1_5.y=94; puntos1.push_back(i1_5);
i2_5.x=136; i2_5.y=63; puntos2.push_back(i2_5);
i1_6.x=172; i1_6.y=94; puntos1.push_back(i1_6);
i2_6.x=162; i2_6.y=70; puntos2.push_back(i2_6);
i1_7.x=151; i1_7.y=120; puntos1.push_back(i1_7);
i2_7.x=130; i2_7.y=90; puntos2.push_back(i2_7);
i1_8.x=172; i1_8.y=118; puntos1.push_back(i1_8);
i2_8.x=157; i2_8.y=95; puntos2.push_back(i2_8);
i1_9.x=151; i1_9.y=143; puntos1.push_back(i1_9);
i2_9.x=126; i2_9.y=112; puntos2.push_back(i2_9);
i1_10.x=148; i1_10.y=170; puntos1.push_back(i1_10);
i2_10.x=120; i2_10.y=140; puntos2.push_back(i2_10);


    Mat imagen1=leeimagen("data/Tablero1.jpg", -1);
    Mat imagen2=leeimagen("data/Tablero2.jpg", -1);
    cout<<"tipo i1"<<imagen1.type()<<endl;
    cout<<"tipo i2"<<imagen2.type()<<endl;
    Mat homografa;
    
    Mat homografia=EstimaHomografia( imagen1, puntos1,puntos2);
    warpPerspective(imagen1,homografa,homografia,imagen1.size());
  
    imshow("original 1",imagen1);
    imshow("homografia calculada",homografa);
    imshow("Tablero 2 original",imagen2);

    
    waitKey(0);
    return 0;
}


