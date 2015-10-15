#include <cstdlib>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "funciones.h"

//using namespace std; 

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

void pintaMI(vector<string> vim) {
    vector<Mat> imagenes;
    int ancho = 0, largo = 0;

    for (int i = 0; i < vim.size(); i++) {
        Mat m = imread(vim[i], IMREAD_UNCHANGED); //leeimagen(vim[i], -1);
        imagenes.push_back(m);
        if (imagenes.at(i).type() != CV_8UC3)
            imagenes.at(i).convertTo(imagenes[i], CV_8UC3);
    }

    for (int i = 0; i < imagenes.size(); i++) {
        ancho += imagenes[i].cols;
        if(imagenes[i].rows>largo)
            largo=imagenes[i].rows;
    }

    Mat fondo(largo, ancho, CV_8UC3, Scalar(0, 0, 0));
    int x = 0;

    for (int i = 0; i < imagenes.size(); i++) {
        Mat roi(fondo, Rect(x, 0, imagenes.at(i).cols, imagenes.at(i).rows));
        imagenes.at(i).copyTo(roi);
        x += imagenes.at(i).cols;
    }

    namedWindow("multiples", WINDOW_AUTOSIZE);
    imshow("multiples", fondo);
    waitKey(0);
    destroyWindow("multiples");
}

void modificaPixel(string im, vector<Point> pt) {
    Mat imagen = leeimagen(im, -1);
    for (int i = 0; i < pt.size(); i++) {
        imagen.at<uchar>(pt.at(i).y, pt.at(i).y) = 0;
    }
    namedWindow("pinta modificada", WINDOW_AUTOSIZE);
    imshow("pinta modificada", imagen);
    waitKey(0);
    destroyWindow("pinta modificada");

}

