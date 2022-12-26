//---------------------------------------------------------------------------

#include "video.h"
#include <math.h>
#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "string.h"

///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES PRIVADAS               //////////////
///////////////////////////////////////////////////////////////////

static VideoCapture camara;
// Capturador de cámara usado actualmente

static Mat img_media;
// Imagen media acumulada de la cámara, con profundidad 32F

static int frames_img_media;
// Número de frames que se han acumulado en la media img_media

string FiltroVideo= "Todos los formatos (*.avi *.mpg *.wmv *.mov);;Archivos AVI (*.avi);;Archivos MPG (*.mpg *.mpeg);;Archivos WMV (*.wmv);;Archivos MOV (*.mov);;Otros (*.*)";

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE VIDEO       //////////////
///////////////////////////////////////////////////////////////////

void rotar_video (int nfoto, string nombre, int modo, int nframes, int codec, double fps)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada && !nombre.empty() && nframes>0);
    Mat rotada;
    rotar_angulo(foto[nfoto].img, rotada, 0, 1, modo);
    VideoWriter writer(nombre, codec, fps, rotada.size());
    if (writer.isOpened()) {
        for (int i= 0; i<=nframes; i++) {
            rotar_angulo(foto[nfoto].img, rotada, i*360.0/nframes, 1, modo);
            namedWindow("Imagen a vídeo - Rotación", WINDOW_NORMAL);
            imshow("Imagen a vídeo - Rotación", rotada);
            waitKey(1);
            writer << rotada;
        }
        writer.release();
        destroyWindow("Imagen a vídeo - Rotación");
    }
}

//---------------------------------------------------------------------------

int inic_camara (int numero)
{
    if (camara.isOpened())
        camara.release();
    camara.open(numero);
    if (camara.isOpened()) {
        Mat img;
        namedWindow("Imagen de cámara", WINDOW_NORMAL);
        camara >> img;
        resizeWindow("Imagen de cámara", img.size().width, img.size().height);
        imshow("Imagen de cámara", img);
        return int(1000.0/30);
    }
    else
        return 0;
}

//---------------------------------------------------------------------------

void fin_camara (void)
{
    if (camara.isOpened()) {
        camara.release();
        namedWindow("Imagen de cámara", WINDOW_NORMAL);
        destroyWindow("Imagen de cámara");
    }
    namedWindow("Imagen media", WINDOW_NORMAL);
    destroyWindow("Imagen media");
}

//---------------------------------------------------------------------------

void acumular_media (bool primera)
{
    assert(camara.isOpened());
    Mat frame;
    camara >> frame;
    namedWindow("Imagen de cámara", WINDOW_NORMAL);
    imshow("Imagen de cámara", frame);
    if (primera) {
        frames_img_media= 1;
        frame.convertTo(img_media, CV_32FC3);
    }
    else {
        frames_img_media++;
        Mat frame32F;
        frame.convertTo(frame32F, CV_32FC3);
        img_media= frame32F + img_media;
        img_media.convertTo(frame, CV_8UC3, 1.0/frames_img_media);
    }
    namedWindow("Imagen media", WINDOW_NORMAL);
    imshow("Imagen media", frame);
}

//---------------------------------------------------------------------------

void media_a_nueva (int nfoto)
{
    Mat res(img_media.size(), CV_8UC3);
    img_media.convertTo(res, CV_8UC3, 1.0/frames_img_media);
    crear_nueva (nfoto, res);
}

//---------------------------------------------------------------------------

void mostrar_camara (void)
{
    Mat img;
    camara >> img;
    namedWindow("Imagen de cámara", WINDOW_NORMAL);
    imshow("Imagen de cámara", img);
}

//---------------------------------------------------------------------------

void capturar_camara(int nres)
{
    VideoCapture cap(0);
    if(cap.isOpened()){
        Mat frame;
        cap.read(frame);
        int tecla=0;
        while((tecla=waitKey(1))== -1 && !frame.empty()){
            namedWindow("Pulse una tecla para capturar",0);
            //mostrar en la ventana anterior la imagen capturada
            imshow("Pulse una tecla para capturar",frame);
            cap.read(frame);

        }
        if(tecla!=27&& !frame.empty())
               crear_nueva(nres,frame);
        destroyWindow("Pulse una tecla para capturar");
    }
    else
        qDebug("No se ha posido abrir la cámara 0");

}

//---------------------------------------------------------------------------

void movimiento (String nombre, int framei, int framef, int nres)

{
    VideoCapture cap(nombre);
    if(cap.isOpened()) {
        cap.set(CAP_PROP_POS_FRAMES, framei); //nos situamos en el principio del vídeo.
        Mat frame,frame_ant;
        Mat dif, dif32;
        Mat acum8;
        if(cap.read(frame)){
            int pos=framei+1;
            // Utilizamos una imagen de 32 bits y 3 canales para evitar el desbordamiento.
            Mat acum(frame.size(),CV_32SC3, Scalar(0,0,0));
            frame.copyTo(frame_ant);

            //El siguiente bucle va leyendo los frames del video y se calcula la diferencia entre el frame en el que estemos
            //y el anterior. Si se llega al final o no se ha podido leer un frame entonces se para de leer en el bucle.
            while(cap.read(frame) && pos<=framef && waitKey(1)==-1){
                pos++;
                //Almacenamos la diferencia dentro de dif.
                absdiff(frame,frame_ant,dif);
                //Convertimos la profundidad de la diferencia para poder almacenarse en la acumulacion.
                dif.convertTo(dif32, CV_32F);
                acum+=dif32;

                //Copiamos el frame actual al anterior, de forma que podamos calcular las siguientes diferencias.
                frame.copyTo(frame_ant);
                imshow("Video",frame);
                normalize(acum,acum8,0,255,NORM_MINMAX,CV_8U);
                imshow("Acumulado",acum8);
             }
          crear_nueva(nres, acum8);
          destroyWindow("Video");
          destroyWindow("Acumulado");

        }


    }



}

//---------------------------------------------------------------------------

void minima_maxima(String nombre, int framei, int framef){
    VideoCapture cap(nombre);
     if(cap.isOpened()) {
         cap.set(CAP_PROP_POS_FRAMES, framei);
         Mat frame, minima,maxima;
         if(cap.read(frame)){
             int pos=framei+1;
             frame.copyTo(minima);
             frame.copyTo(maxima);
             while(cap.read(frame) && pos<=framef && waitKey(1)==-1){
                 //Calculamos el minimo y el maximo
                    pos++;
                    min(minima,frame,minima);
                    max(maxima,frame,maxima);

             }
             crear_nueva(primera_libre(),minima);
             crear_nueva(primera_libre(),maxima);

         }

     }
}

void ver_caras_video(String nombre){
    VideoCapture cap;

    if(nombre.compare("cam")==0){
        cap.open(0);
    }
    else{
        cap.open(nombre);
    }
    CascadeClassifier cascade("C:/OpenCV/OpenCV4.6.0G/data/haarcascades/haarcascade_frontalface_alt.xml");
    std::vector<Rect> caras;
    Mat img;
    QString nombre_resultado= QFileDialog::getSaveFileName();


    if(cap.isOpened()){

    //tratamos el primer frame
    cap.read(img);
    VideoWriter writer(nombre_resultado.toLatin1().data(),VideoWriter::fourcc('M','J','P','G'),
                       25, img.size());
    if (!writer.isOpened()) {
     qDebug("No se puede crear %s.", nombre_resultado.toLatin1().data());
     return;
     }

    cascade.detectMultiScale(img, caras, 1.2);
    for (int i= 0; i<caras.size(); i++){
      rectangle(img, caras[i], CV_RGB(255,0,0), 2);
    }
    writer <<img;

    //tratamos el resto de frames
    while (cap.read(img) && waitKey(1)==-1) {
           cascade.detectMultiScale(img, caras, 1.2);
           for (int i= 0; i<caras.size(); i++){
             rectangle(img, caras[i], CV_RGB(255,0,0), 2);
           }
           namedWindow("Pulsa un botón para terminar el proceso", WINDOW_NORMAL);
           imshow("Imagen", img);
           writer << img;


    }
    destroyWindow("Imagen");
    writer.release();

}

}

void ver_caras(String nombre, bool guardar){
    VideoCapture cap(nombre);
    CascadeClassifier cascade("C:/OpenCV/OpenCV4.6.0G/data/haarcascades/haarcascade_frontalface_alt.xml");
    std::vector<Rect> caras;
    std::vector<Mat> imagenes_caras;
    Mat img,res,temp;




    //Detectamos las caras
    while (cap.read(img) && waitKey(1)==-1) {
           cascade.detectMultiScale(img, caras, 1.2);
           for (int i= 0; i<caras.size(); i++){
               temp=img(caras[i]);
               resize(temp, temp, Size(50,50), 0, 0, INTER_LINEAR);
                imagenes_caras.push_back(temp);
           }


    }

    //Calculamos el número de filas que debemos rellenar.
    int num_images = ceil(sqrt(imagenes_caras.size()));

    //Array de filas
    vector<Mat> filas;

    for (int i = 0; i <num_images; i++) {
        // Crea una fila de imágenes
        vector<Mat> fila;
        for (int j = i * num_images; j < (i + 1) * num_images; j++) {
          if (j >= imagenes_caras.size()) {
            // Si nos hemos quedado sin imagenes, rellenamos con un cuadrado blanco hasta llegar al final
            fila.push_back(Mat::zeros(Size(50, 50), CV_8UC3));
          } else {
            // Añade la imagen a la fila
            fila.push_back(imagenes_caras[j]);
          }
        }

        //A partir de todas las imagenes que hemos generado de caras y que tenemos en el array
        //generamos una imagen que represente a una fila.
          Mat imagen_fila;
          hconcat(fila, imagen_fila);

          // Añadimos la imagen de fila generada al total de filas
          filas.push_back(imagen_fila);
    }


    vconcat(filas,res);

    if(guardar){
        crear_nueva(primera_libre(),res);
    }
    else{
    namedWindow("Imagen", WINDOW_NORMAL);
    imshow("Imagen",res);
    }


}



