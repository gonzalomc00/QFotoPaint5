//---------------------------------------------------------------------------

#include "video.h"
#include <math.h>
#include <QtDebug>

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
    //Podemos mejorarlo permitiendo al usuario que elija la camara con la que abrirlo
    VideoCapture cap(0);
    if(cap.isOpened()){
        Mat frame;
        cap.read(frame);
        int tecla=0;
        //mientras no se haya pulsado una tecla seguimos mostrando. Cuando pulsemos una tecla tomamos la captura
        //si el frame no muestra nada entonces tampoco podemos coger la captura
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
        if(cap.read(frame)){  //leemos el frame en el que nos situamos
            int pos=framei+1;
            Mat acum(frame.size(),CV_32SC3, Scalar(0,0,0)); //tenemos una imagen de 32 bits con signo de forma que la acumulación no pueda desbordarse.
            frame.copyTo(frame_ant);

            //el siguiente bucle va a ir leyendo los frames y se calcula la diferencia entre el frame en el que estemos
            //y el anterior. Si se llega al final o no se ha podido leer un frame entonces se para de leer en el bucle.
            while(cap.read(frame) && pos<=framef && waitKey(1)==-1){
                pos++;
                absdiff(frame,frame_ant,dif); //almacenamos la diferencia dentro de if.
                dif.convertTo(dif32, CV_32F); //convertimos la profundidad de la diferencia para poder almacenarse en la diferencia.
                acum+=dif32; //sumamos dif32 de forma que podamos acumular.

                frame.copyTo(frame_ant); //copiamos el frame actual al anterior, de forma que podamos calcular las siguientes diferencias.
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
