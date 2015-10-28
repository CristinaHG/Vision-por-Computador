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

Mat calculaMascara(float sigma) {

    float intervalo = 3 * sigma;
    float normalizacion;
    vector<float> inter;

    for (int i = -intervalo; i <= intervalo; i++) {
        inter.push_back((float) i);
    }

    Mat mascara(1, inter.size(), DataType<float>::type);

    for (int i = 0; i < inter.size(); i++) {
        mascara.at<float>(0, i) = inter.at(i);
    }

    float suma = 0;
    for (int i = 0; i < mascara.cols; i++) {
        mascara.at<float>(0, i) = (float) exp(-0.5 * ((mascara.at<float>(0, i) * mascara.at<float>(0, i)) / (sigma * sigma)));
        suma += mascara.at<float>(0, i);
    }
    //normalizacion
    for (int i = 0; i < mascara.cols; i++) {
        mascara.at<float>(0, i) = (mascara.at<float>(0, i) *(1 / suma));
    }
    //comprobar que la máscara suma 1
    cout << "suma mascara " << sum(mascara) << endl;

    return mascara;

}

Mat calculaConvol1D_columnas(Mat imagenEntrada, Mat mascara, int contorno) {
    
vector<Mat> canalesEntrada;
vector<Mat> canalesSalida;
int tipoImagen = imagenEntrada.type();
int tipoMascara=mascara.type();


split(imagenEntrada,canalesEntrada);
cout<<"tamaño canalesEntrada"<<canalesEntrada.size() <<endl;

    //    Mat salida(1, imagenEntrada.cols, CV_32F);
    Mat salida = imagenEntrada.clone();
    //    imagenEntrada.convertTo(imagenEntrada, CV_32F);
    //  mascara.convertTo(mascara, CV_32F);

    int sobrante = mascara.cols / 2;
    int columnasAuxiliar = imagenEntrada.cols + 2 * sobrante;
    
    for(int j=0 ;j < canalesEntrada.size();j++){
         
        canalesEntrada.at(j).convertTo(canalesEntrada.at(j),tipoMascara);
        //canalesSalida.at(j).convertTo(canalesSalida.at(j),tipoImagen);
        Mat m= canalesEntrada.at(j).clone();
        //cout<<"m"<<m;
        canalesSalida.push_back(m);
     
        Mat auxiliar; //(1, columnasAuxiliar, CV_32F);
        if (contorno == 0)
            copyMakeBorder(canalesEntrada.at(j), auxiliar, 0, 0, sobrante, sobrante, BORDER_CONSTANT, Scalar(0));
        else if (contorno == 1)
            copyMakeBorder(canalesEntrada.at(j), auxiliar, 0, 0, sobrante, sobrante, BORDER_REFLECT);


        Mat aux = canalesEntrada.at(j).clone(); //1, imagenEntrada.cols, CV_32F);

        Mat roi(auxiliar, Rect(0, 0, mascara.cols, 1));
        for (int i = 0; i < canalesSalida.at(j).cols ; i++) {
            aux = roi.mul(mascara);
            canalesSalida.at(j).at<float>(i)=(float) *(sum(aux).val);
            roi = roi.adjustROI(0, 0, -1, 1);
        }
    }
    
    merge(canalesSalida,salida);
    salida.convertTo(salida,tipoImagen);
    //salida.copyTo(imagenEntrada);

    //if (contorno==0){ //contorno uniforme a ceros
    /*   for (int i=0; i <auxiliar.size(); i++){
           for (int j=0;j<imagenEntrada.size();j++){
               if(i< sobrante || i> (imagenEntrada.size()+(sobrante-1))){
                   auxiliar.at<float>(0, i)=0;
               }else{
                   auxiliar.at<float>(0, i)=j;
               }
           }
       }
   }
     */


    /*
}else if (contorno==1){ //contorno reflejada
    for(int i=sobrante; i>-1;i--){
        for (int j=imagenEntrada;j< imagenEntrada.size();j++){
            auxiliar.at<float>(0, i)=auxiliar.at<float>(0, j);     
        }
    }
     */
    return salida;
    //calculo convolucion
}

Mat calculaConvol1D_filas(Mat imagenEntrada, Mat mascara, int contorno) {
    
vector<Mat> canalesEntrada;
vector<Mat> canalesSalida;
int tipoImagen = imagenEntrada.type();
int tipoMascara=mascara.type();

split(imagenEntrada,canalesEntrada);
    mascara = mascara.t();
    //    Mat salida(1, imagenEntrada.cols, CV_32F);
    Mat salida = imagenEntrada.clone();
    //    imagenEntrada.convertTo(imagenEntrada, CV_32F);
    //  mascara.convertTo(mascara, CV_32F);

    int sobrante = mascara.rows / 2;
    int columnasAuxiliar = imagenEntrada.rows + 2 * sobrante;

    for(int j=0 ;j < canalesEntrada.size();j++){
        canalesEntrada.at(j).convertTo(canalesEntrada.at(j),tipoMascara);
        Mat m= canalesEntrada.at(j).clone();
        //cout<<"m"<<m;
        canalesSalida.push_back(m);
        
        Mat auxiliar; //(1, columnasAuxiliar, CV_32F);
        if (contorno == 0)
            copyMakeBorder(canalesEntrada.at(j), auxiliar,sobrante, sobrante,0,0, BORDER_CONSTANT, Scalar(0));
        else if (contorno == 1)
            copyMakeBorder(canalesEntrada.at(j), auxiliar,sobrante, sobrante,0,0, BORDER_REFLECT);


        Mat aux = canalesEntrada.at(j).clone(); //1, imagenEntrada.cols, CV_32F);

        Mat roi(auxiliar, Rect(0, 0, 1, mascara.rows));
        for (int i = 0; i < canalesSalida.at(j).rows; i++) {
            aux = roi.mul(mascara);
            canalesSalida.at(j).at<float>(i)= (float) *(sum(aux).val);
            roi = roi.adjustROI(-1, 1, 0, 0);
        }
    }
merge(canalesSalida,salida);
salida.convertTo(salida,tipoImagen);
    //salida.copyTo(imagenEntrada);

    //if (contorno==0){ //contorno uniforme a ceros
    /*   for (int i=0; i <auxiliar.size(); i++){
           for (int j=0;j<imagenEntrada.size();j++){
               if(i< sobrante || i> (imagenEntrada.size()+(sobrante-1))){
                   auxiliar.at<float>(0, i)=0;
               }else{
                   auxiliar.at<float>(0, i)=j;
               }
           }
       }
   }
     */


    /*
}else if (contorno==1){ //contorno reflejada
    for(int i=sobrante; i>-1;i--){
        for (int j=imagenEntrada;j< imagenEntrada.size();j++){
            auxiliar.at<float>(0, i)=auxiliar.at<float>(0, j);     
        }
    }
     */
    return salida;
    //calculo convolucion
}

//Mat calculaConvol1D_filas(Mat imagenEntrada, Mat mascara, int contorno) {
//
//    imagenEntrada.convertTo(imagenEntrada, CV_32F);
//    //mascara.convertTo(mascara, CV_32F);
//
//    //Mat imagenEntrada(imagenEntrada.cols,1, CV_32F);
//    //Mat mascara(mascara.cols,1, CV_32F);
//    //  
//    //transpose(imagenEntrada,imagenEntrada);
//    //imagenEntrada.t();
//    //transpose(mascara,mascara);
//    //
//    mascara = mascara.t();
//
//
//    Mat salida(imagenEntrada.rows, 1, CV_32F);
//
//    int sobrante = mascara.rows / 2;
//    int filasAuxiliar;
//    filasAuxiliar = imagenEntrada.rows + 2 * sobrante;
//
//    Mat auxiliar(filasAuxiliar, 1, CV_32F);
//
//    if (contorno == 0)
//        copyMakeBorder(imagenEntrada, auxiliar, sobrante, sobrante, 0, 0, BORDER_CONSTANT, Scalar(0));
//    else if (contorno == 1)
//        copyMakeBorder(imagenEntrada, auxiliar, sobrante, sobrante, 0, 0, BORDER_REFLECT);
//
//    Mat aux(imagenEntrada.rows, 1, CV_32F);
//
//    Mat roi(auxiliar, Rect(0, 0, 1, mascara.rows));
//    for (int i = 0; i < salida.rows; i++) {
//        aux = roi.mul(mascara);
//        salida.at<float>(i) = (float) *(sum(aux).val);
//        roi = roi.adjustROI(-1, 1, 0, 0);
//    }
//
//    return salida;
//}

Mat convolucion2D(string imagen2D, float sigma, int contorno) {
    Mat mascara = calculaMascara(sigma);

    Mat imagenEntrada;

    imagenEntrada = leeimagen(imagen2D, -1);
    int tipo = imagenEntrada.type();
    int tipom=mascara.type();
    imshow("Antes", imagenEntrada);

    imagenEntrada.convertTo(imagenEntrada,tipom);
    //    mascara.convertTo(mascara, CV_32F);

    
    Mat aux(imagenEntrada.cols, 1, tipom);
    Mat imagen1D_V(imagenEntrada.rows, imagenEntrada.cols, tipo);
    for (int i = 0; i < imagenEntrada.cols; i++) {
        //        cout<<"imagenEntrada.col(i)="<<imagenEntrada.col(0)<<endl;
        aux = calculaConvol1D_filas(imagenEntrada.col(i), mascara, contorno);
        //        cout<<"aux: "<<aux;
        aux.copyTo(imagen1D_V.col(i));
    }
     
    Mat aux2(1, imagenEntrada.rows, tipom);
    Mat salida(imagenEntrada.rows, imagenEntrada.cols,tipo);
    
    for (int i = 0; i < imagenEntrada.rows; i++) {
        //aux2 = calculaConvol1D_columnas(imagen1D_V.row(i), mascara, contorno);
        aux2 = calculaConvol1D_columnas(imagen1D_V.row(i), mascara, contorno);
        //cout<<"aux vale"<<aux2<<endl;
        aux2.copyTo(salida.row(i));
    }
    //calculaConvol1D_columnas(imagenEntrada,mascara,contorno).copyTo(imagen1D_V);
    //calculaConvol1D_filas(imagen1D_V,mascara,contorno).copyTo(salida);
    salida.convertTo(salida, tipo);
    return salida;
    //    imagen1D_V.convertTo(imagen1D_V, CV_8SC3);
    //    return imagen1D_V;

}

