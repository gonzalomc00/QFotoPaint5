//---------------------------------------------------------------------------

#include "imagenes.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <assert.h>




///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES                        //////////////
///////////////////////////////////////////////////////////////////

ventana foto[MAX_VENTANAS];

tipo_herramienta herr_actual= HER_PUNTO;

int radio_pincel= 10;

Scalar color_pincel= CV_RGB(255, 255, 255);

int difum_pincel= 10;

bool preguntar_guardar= true;

static int numpos= 0; // Número actual en el orden de posición de las ventanas

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE MANEJO DE VENTANAS           //////////////
///////////////////////////////////////////////////////////////////

void callback (int event, int x, int y, int flags, void *_nfoto);

//---------------------------------------------------------------------------

//Inicializamos el array que contiene las imagenes. No tienen porque tener una foto si no que puede estar a la espera
//de una.
void inic_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        foto[i].usada= false;
}

//---------------------------------------------------------------------------

void fin_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            destroyWindow(foto[i].nombre);
            foto[i].usada= false;
        }
    }
}

//---------------------------------------------------------------------------

int primera_libre (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        if (!foto[i].usada)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

void escribir_barra_estado (void)
{
    int usadas, modificadas;
    num_fotos(usadas, modificadas);
    w->setStatusBarText(QString::number(usadas)+" fotos abiertas, "+
                        QString::number(modificadas)+" modificadas.");
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, int ancho, int alto, Scalar color)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].nombre= "nueva-"+to_string(nfoto)+".png";
    foto[nfoto].nombref= foto[nfoto].nombre;
    foto[nfoto].img.create(alto, ancho, CV_8UC3);
    foto[nfoto].img=  color;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(0, 0, ancho, alto);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, Mat img)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada && !img.empty());
    foto[nfoto].nombre= "nueva-"+to_string(nfoto)+".png";
    foto[nfoto].nombref= foto[nfoto].nombre;
    foto[nfoto].img= img;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(0, 0, img.cols, img.rows);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].nombre= nombre;
    foto[nfoto].nombref= Lt1(nombre);
    foto[nfoto].img= imread(foto[nfoto].nombref);
    if (foto[nfoto].img.empty())
        return;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= false;
    foto[nfoto].roi= Rect(0, 0, foto[nfoto].img.cols, foto[nfoto].img.rows);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void guardar_foto (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    foto[nfoto].modificada= false;
    // 1. Guardar la imagen con el mismo nombre que tiene
    if (nombre=="")
        imwrite(foto[nfoto].nombref, foto[nfoto].img);
    // 2. Guardar la imagen con un nombre distinto al actual
    else {
        imwrite(Lt1(nombre), foto[nfoto].img);
        setWindowTitle(foto[nfoto].nombre, nombre);
    }
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void cerrar_foto (int nfoto, bool destruir_ventana)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    if (destruir_ventana)
        destroyWindow(foto[nfoto].nombre);
    foto[nfoto].usada= false;
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void mostrar (int nfoto)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

int nombre_a_numero (string nombre)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        if (foto[i].usada && foto[i].nombre==nombre)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

int foto_activa (void)
{
    int maxorden= -1, maxpos= -1;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada && foto[i].orden>maxorden) {
            maxorden= foto[i].orden;
            maxpos= i;
        }
    }
    return maxpos;
}

//---------------------------------------------------------------------------

int num_fotos (int &usadas, int &modificadas)
{
    usadas= 0;
    modificadas= 0;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            usadas++;
            if (foto[i].modificada)
                modificadas++;
        }
    }
    return usadas;
}

//---------------------------------------------------------------------------

void set_callback_foto (int nfoto, bool activo)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    if (!activo)
        setMouseCallback(foto[nfoto].nombre, nullptr);
    else
        setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
}

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DEL CALLBACK PRINCIPAL          //////////////
///////////////////////////////////////////////////////////////////

static int downx, downy;
// Posición inicial del ratón al pinchar sobre la imagen actual

//---------------------------------------------------------------------------

void ninguna_accion (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    circle(res, Point(x, y), radio_pincel, CV_RGB(255,255,255), 2, LINE_AA);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_close (int factual)
{
    if (foto[factual].usada && !foto[factual].img.empty()) {
        if (foto[factual].modificada && preguntar_guardar) {
            QString cadena= "El archivo " + QString::fromStdString(foto[factual].nombre) +
                    " ha sido modificado.\n¿Desea guardarlo?";
            int resp= QMessageBox::question(w, "Archivo modificado", cadena,
                                            QMessageBox::Yes | QMessageBox::No );
            if (resp==QMessageBox::Yes)
                guardar_foto(factual);
        }
        cerrar_foto(factual, false);
    }
}

//---------------------------------------------------------------------------

void cb_punto (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, color_pincel, -1, LINE_AA);
    else {
        int tam= radio_pincel+difum_pincel;
        int posx= tam, posy=tam;
        Rect roi(x-tam,y-tam,2*tam+1,2*tam+1);
        if(roi.x<0){
            roi.width+=roi.x;
            posx+=roi.x;
            roi.x=0;
        }
        if(roi.y<0) {
               roi.height+=roi.y;
               posy+=roi.y;
               roi.y=0;
        }
        if(roi.x+roi.width > im.cols){
            roi.width=im.cols-roi.x;
        }
        if(roi.y+roi.height > im.rows){
            roi.height=im.rows-roi.y;
        }
        Mat frag=im(roi);
        Mat res(frag.size(), frag.type(), color_pincel);
        Mat cop(frag.size(), frag.type(), CV_RGB(0,0,0));
        circle(cop, Point(posx, posy), radio_pincel, CV_RGB(255,255,255), -1, LINE_AA);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(frag, cop, frag, 1.0/255.0);
        frag= res + frag;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------


Scalar ColorArcoIris ()
{
    static Scalar colorActual=CV_RGB(255,0,0);
    static int estado=0;
    switch(estado){
        case 0:
            if(++colorActual.val[1]>=255) estado=1;
            break;
        case 1:
            if(--colorActual.val[2]<=0) estado=2;
            break;
        case 2:
            if(++colorActual.val[0]>=255) estado=3;
            break;
        case 3:
            if(--colorActual.val[1]<=0) estado = 4;
            break;
        case 4:
            if(++colorActual[2]>=255) estado=5;
            break;
        case 5:
            if(--colorActual.val[0]<=0) estado=0;

    }
    return colorActual;
    }




void cb_arcoiris (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, ColorArcoIris(), -1, LINE_AA);
    else {
        int tam= radio_pincel+difum_pincel;
        int posx= tam, posy=tam;
        Rect roi(x-tam,y-tam,2*tam+1,2*tam+1);
        if(roi.x<0){
            roi.width+=roi.x;
            posx+=roi.x;
            roi.x=0;
        }
        if(roi.y<0) {
               roi.height+=roi.y;
               posy+=roi.y;
               roi.y=0;
        }

        if(roi.x+roi.width > im.cols){
            roi.width=im.cols-roi.x;
        }
        if(roi.y+roi.height > im.rows){
            roi.height=im.rows-roi.y;
        }
        Mat frag=im(roi);
        Mat res(frag.size(), frag.type(), ColorArcoIris());
        Mat cop(frag.size(), frag.type(), CV_RGB(0,0,0));
        circle(cop, Point(posx, posy), radio_pincel, CV_RGB(255,255,255), -1, LINE_AA);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(frag, cop, frag, 1.0/255.0);
        frag= res + frag;
    }
    //la imagen completa se sigue mostrando ?
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}


//---------------------------------------------------------------------------

void cb_linea (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        line(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2+1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        line(cop, Point(downx, downy), Point(x,y), CV_RGB(255,255,255), radio_pincel*2+1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_ver_linea (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    line(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2+1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------
void cb_ver_rectangulo (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    rectangle(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------
void cb_ver_elipse (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    //downx/y es donde hemos hecho click
    ellipse(res, Point(downx, downy), Size(abs(x-downx),abs(y-downy)),0,0,360, color_pincel, radio_pincel*2-1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_rectangulo (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        rectangle(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        rectangle(cop, Point(downx, downy), Point(x,y), CV_RGB(255,255,255), radio_pincel*2-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}




//---------------------------------------------------------------------------
void cb_elipse (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        ellipse(im, Point(downx, downy), Size(abs(x-downx),abs(y-downy)),0,0,360, color_pincel, radio_pincel*2-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        ellipse(cop, Point(downx, downy), Size(abs(x-downx),abs(y-downy)),0,0,360, CV_RGB(255,255,255), radio_pincel*2-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}


//---------------------------------------------------------------------------

void cb_seleccionar (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    Rect nuevo= Rect(min(downx, x), min(downy, y),
                     max(downx, x)-min(downx, x)+1, max(downy, y)-min(downy, y)+1);
    if (nuevo.x<0)
        nuevo.x= 0;
    if (nuevo.y<0)
        nuevo.y= 0;
    if (nuevo.x+nuevo.width>im.size().width)
        nuevo.width= im.size().width-nuevo.x;
    if (nuevo.y+nuevo.height>im.size().height)
        nuevo.height= im.size().height-nuevo.y;
    foto[factual].roi= nuevo;
}

//---------------------------------------------------------------------------

void cb_ver_seleccion (int factual, int x, int y, bool foto_roi)
{
    Mat res= foto[factual].img.clone();
    Point p1, p2;
    if (foto_roi) {
        p1.x= foto[factual].roi.x;
        p1.y= foto[factual].roi.y;
        p2.x= foto[factual].roi.x+foto[factual].roi.width-1;
        p2.y= foto[factual].roi.y+foto[factual].roi.height-1;
    }
    else {
        p1= Point(downx, downy);
        p2= Point(x, y);
    }
    rectangle(res, p1, p2, CV_RGB(255,foto_roi?0:255,0),2);
    imshow(foto[factual].nombre, res);
}




//---------------------------------------------------------------------------

//Se encarga de tratar las acciones de los inputs.
void callback (int event, int x, int y, int flags, void *_nfoto)
{
    int factual= reinterpret_cast<int>(_nfoto);

    // 1. Eventos y casos especiales
    // 1.1. Evento cerrar ventana
    if (event==EVENT_CLOSE) {
        cb_close(factual);
        return;
    }
    // 1.2. Evento obtiene el foco
    if (event==EVENT_FOCUS) {
        foto[factual].orden= numpos++;
    }
    // 1.3. El ratón se sale de la ventana
    if (x<0 || x>=foto[factual].img.size().width || y<0 || y>=foto[factual].img.size().height)
        return;
    // 1.4. Se inicia la pulsación del ratón
    if (event==EVENT_LBUTTONDOWN) {
        downx= x;
        downy= y;
    }

    // 2. Según la herramienta actual
    switch (herr_actual) {

    // 2.1. Herramienta PUNTO
    case HER_PUNTO:
        if (flags==EVENT_FLAG_LBUTTON)
            cb_punto(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.2. Herramienta LINEA
    case HER_LINEA:
        if (event==EVENT_LBUTTONUP)
            cb_linea(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_linea(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.3. Herramienta SELECCION
    case HER_SELECCION:
        if (event==EVENT_LBUTTONUP)
            cb_seleccionar(factual, x, y);
        else if (event==EVENT_MOUSEMOVE)
            cb_ver_seleccion(factual, x, y, flags!=EVENT_FLAG_LBUTTON);
        break;

        // 2.4. Herramienta RECTANGULO
    case HER_RECTANGULO:
        if (event==EVENT_LBUTTONUP)
            cb_rectangulo(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_rectangulo(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.5. Herramienta ELIPSE
    case HER_ELIPSE:
        if (event==EVENT_LBUTTONUP)
            cb_elipse(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_elipse(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.6. Herramienta PUNTO
     case HER_ARCOIRIS:
            if (flags==EVENT_FLAG_LBUTTON)
                cb_arcoiris(factual, x, y);
            else
                ninguna_accion(factual, x, y);
            break;

    }


    escribir_barra_estado();
}


///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE IMAGENES    //////////////
///////////////////////////////////////////////////////////////////

void invertir (int nfoto, int nres)
{
    Mat img(foto[nfoto].img.size(), foto[nfoto].img.type());
    bitwise_not(foto[nfoto].img, img);
    crear_nueva(nres, img);
}

//---------------------------------------------------------------------------

void rotar_angulo (Mat imagen, Mat &imgRes, double angulo, double escala, int modo)
{
    double w= imagen.size().width, h= imagen.size().height;
    Size sres;
    if (modo==0) {     // Reescalar con proporción al original
        sres.width= int(w*escala);
        sres.height= int(h*escala);
    }
    else if (modo==1)  // Reescalar y ampliar para caber entera
        sres.width= sres.height= int(sqrt(w*w + h*h)*escala);
    else               // Reescalar y recortar para no mostrar borde
        sres.width= sres.height= int(min(w, h)*escala/sqrt(2.0));
    imgRes.create(sres, imagen.type());
    double sa= sin(angulo*M_PI/180), ca= cos(angulo*M_PI/180);
    double cx= -w/2*ca-h/2*sa, cy= w/2*sa-h/2*ca;
    Mat M= getRotationMatrix2D(Point2f(0,0), angulo, escala);
    M.at<double>(0,2)= sres.width/2+cx*escala;
    M.at<double>(1,2)= sres.height/2+cy*escala;
    imgRes= color_pincel;
    warpAffine(imagen, imgRes, M, sres, INTER_CUBIC);
}

//---------------------------------------------------------------------------

void rotar_exacto (int nfoto, int nres, int grado)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    assert(nres>=0 && nres<MAX_VENTANAS && !foto[nres].usada);
    Mat entrada= foto[nfoto].img;
    Mat salida;
    if (grado == 0)
        salida= entrada.clone();
    else if (grado == 1) {
        transpose(entrada, salida);
        flip(salida, salida, 1);
    }
    else if (grado == 2)
        flip(entrada, salida, -1);
    else if (grado == 3) {
        transpose(entrada, salida);
        flip(salida, salida, 0);
    }
    crear_nueva(nres, salida);
}

//---------------------------------------------------------------------------

void ver_brillo_contraste (int nfoto, double suma, double prod,double gama, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    Mat img;
    foto[nfoto].img.convertTo(img, CV_8UC3, prod, suma);
    Mat img32f;
    img.convertTo(img32f,CV_32F,1.0/255);
    pow(img32f,gama,img32f);
    img32f.convertTo(img,CV_8U,255);

    imshow(foto[nfoto].nombre, img);
    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void ver_suavizado (int nfoto, int ntipo, int tamx, int tamy, bool guardar)
{
    //función que se encarga del suavizado y elegimos si guardarla o cancelar
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    assert(tamx>0 && tamx&1 && tamy>0 && tamy&1);

    Mat img= foto[nfoto].img.clone();
    Mat fragmento=img(foto[nfoto].roi);
    //Si el suavizado es de tipo 1 se llama al filtro gaussiano
    if (ntipo == 1)
        GaussianBlur(fragmento,fragmento, Size(tamx, tamy), 0);
    else if (ntipo == 2)
        blur(fragmento, fragmento, Size(tamx, tamy));
    else if (ntipo==3)
        medianBlur(fragmento,fragmento, min(tamx,301)); //si es mayor que 301 le aplicamos ese suavizado para no sobrepasarnos y que de error

    imshow(foto[nfoto].nombre, img);

    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void media_ponderada (int nf1, int nf2, int nueva, double peso)
{
    assert(nf1>=0 && nf1<MAX_VENTANAS && foto[nf1].usada);
    assert(nf2>=0 && nf2<MAX_VENTANAS && foto[nf2].usada);
    assert(nueva>=0 && nueva<MAX_VENTANAS && !foto[nueva].usada);
    Mat img= foto[nf1].img.clone();
    Mat cop;
    resize(foto[nf2].img, cop, img.size());
    addWeighted(img, peso, cop, 1.0-peso, 0, img);
    crear_nueva(nueva, img);
}


//---------------------------------------------------------------------------
void ver_histograma(int nfoto, int nres, int canal){

    QImage imq= QImage(":/imagenes/histbase.png");
    Mat imghist(imq.height(),imq.width(),CV_8UC4,imq.scanLine(0));
    cvtColor(imghist, imghist, COLOR_RGBA2RGB);
    Mat gris;
    Mat hist;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);  // Conversión a gris
    int canales[1]= {0};
    int bins[1]= {256};
    float rango[2]= {0, 256};
    const float *rangos[]= {rango};
    double vmin, vmax;

    //primero calculamos el histograma y luego calculamos el maximo y el minimo
    if(canal==3){ // calculamos sobre gris
    calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);
    } else{
        //calculamos sobre el canal
        calcHist(&(foto[nfoto].img), 1, &canal, noArray(), hist, 1, bins, rangos);
    }
    minMaxLoc(hist, &vmin, &vmax); //función que calcula el mínimo y el máximo del histograma de forma que el histograma se escale en torno a estos valores

    for (int i= 0; i<256; i++){
        //escalamos el histograma y le damos la vuelta
        float poshist=185-hist.at<float>(i)/vmax*182;
        //ensanchamos el histograma a lo largo del eje x y lo dibujamos
        rectangle(imghist,Point(3+i*391.0/256,185),Point(3+(i+1)*391.0/256,poshist), CV_RGB(canal==2?255:0,canal==1?255:0,canal==0?255:0),-1);
    }

    crear_nueva(nres,imghist);


}
//---------------------------------------------------------------------------

void ver_bajorrelieve(int nfoto, double angulo, double grado, int fondo,
                      bool guardar)
{

    QString nombres[4]={":/imagenes/arena.jpg",":/imagenes/cielo.jpg",
                       ":/imagenes/gris.png",":/imagenes/madera.jpg"};



    //dependiendo del parametro fondo se va escogiendo una foto u otra
    //es necesario reescalar la imagen.
    QImage imq= QImage(nombres[fondo]);
    Mat imgfondo(imq.height(),imq.width(),CV_8UC4,imq.scanLine(0));
    cvtColor(imgfondo, imgfondo, COLOR_RGBA2RGB);
    resize(imgfondo,imgfondo,foto[nfoto].img.size()); //reescalado

    //Convertimos la imagen a gris, le pasamos el filtro de sobel
    Mat gris; //imagen original
    cvtColor(foto[nfoto].img, gris,COLOR_BGR2GRAY);

    //Rotamos la imagen en gris (para cambiar la direccion)
    Mat rotada;
    //rotamos la imagen de manera que podamos calcular la derivada en cualquier angulo
    rotar_angulo(gris,rotada,angulo,1.0,1);//al calcular la derivada en x, se pierde un trozo; con el modo 1, la imagen se aumenta para que quepa entera en la imagen rotada
    Mat sobel;
    Sobel(rotada,sobel,CV_8U,1,0,3,grado, 128);
    //tras aplicar el filtro de Sobel, devolvemos la imagen al angulo original
    rotar_angulo(sobel, rotada,-angulo,1.0,0);
    //Para eliminar el borde exterior, recortamos la imagen, de forma que solo obtengamos la parte util de esta.
    gris= rotada(Rect((rotada.cols-gris.cols)/2,(rotada.rows-gris.rows)/2,gris.cols,gris.rows));


    Mat array[3]={gris,gris,gris};
    Mat res;
    merge(array,3,res);

    addWeighted(imgfondo, 1.0, res , 1.0, -128, res);

    if(guardar){
        crear_nueva(primera_libre(),res);
    }
    else{
        imshow("Bajorrelieve",res);
    }

}
//---------------------------------------------------------------------------
void ver_rotar_cualquiera(int nfoto, int angulo, double escala, bool guardar){

    Mat entrada= foto[nfoto].img;
    Mat res;

    double w= entrada.size().width, h= entrada.size().height;
    double sa= sin(angulo*M_PI/180), ca= cos(angulo*M_PI/180);
    double cx= -w/2*ca-h/2*sa, cy= w/2*sa-h/2*ca;
    sa= fabs(sa); ca= fabs(ca);

    Size tam((w*ca+h*sa)*escala, (h*ca+w*sa)*escala);

    Mat M= getRotationMatrix2D(Point2f(0,0), angulo, escala);
    M.at<double>(0,2)= tam.width/2+cx*escala;
    M.at<double>(1,2)= tam.height/2+cy*escala;
    warpAffine(entrada, res, M, tam);

    imshow("Rotacion",res);

    if(guardar){
        crear_nueva(primera_libre(),res);

    }else{
        imshow("Rotacion",res);
    }
}

//---------------------------------------------------------------------------
void ver_ajuste_lineal(int nfoto, double pmin, double pmax, bool guardar){

//Cogemos la imagen, la convertimos a gris, calculamos el histograma, con el histograma calculamos los percentiles y luego
//hacemos la operación de estiramiento lineal.
    Mat gris;
    cvtColor(foto[nfoto].img,gris,COLOR_BGR2GRAY);
    int canales[1]={0};
    int bins[1]={256};
    float rango[2]={0,256};
    const float *rangos[]={rango};
    Mat hist;
    calcHist(&gris,1,canales,noArray(),hist,1,bins,rangos);


    //Escala los valores del histograma entre 0 y 100.
    normalize(hist,hist, 100, 0, NORM_L1);

    double acum=0;
    int vmin=0;

    //Calculamos el percentil mínimo
    for(;vmin<256 && acum<pmin;vmin++)
        acum+=hist.at<float>(vmin);

    acum=0;

    //Calculamos el percentil máximo
    int vmax=255;
    for(; vmax>=0 && acum<pmax; vmax--)
        acum+=hist.at<float>(vmax);

    //Corregimos el valor de los percentiles en caso de que sea necesario
    if(vmin>=vmax) vmax=vmin+1;
    //Calculamos los valores de a y b
    double a= 255.0/(vmax-vmin);
    double b= -vmin*a;

    Mat res;
    //Estiramos el histograma
    foto[nfoto].img.convertTo(res,CV_8U,a,b);
    imshow(foto[nfoto].nombre,res);
    if(guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada=true;
    }

}

//---------------------------------------------------------------------------

void escala_color(int nfoto, int nres){

    Mat gris;
    cvtColor(foto[nfoto].img,gris,COLOR_BGR2GRAY);
    //Convertimos la imagen de gris a RGB de manera que tripliquemos el canal gris
    cvtColor(gris,gris,COLOR_GRAY2BGR);

    //Creamos una tabla LUT que contendrá tres canales de 8 bits cada uno
    Mat lut(1,256,CV_8UC3);
    int vb=color_pincel.val[0]; //color azul del pincel
    int vg=color_pincel.val[1]; //color verde del pincel
    int vr=color_pincel.val[2]; //color rojo del pincel
    //Calculamos los valores de la escala del nuevo color
    for(int A=0;A<256;A++){
        if(A<128){
            lut.at<Vec3b>(A)= Vec3b(vb*A/128, vg*A/128, vr*A/128);
        }
        else {
            lut.at<Vec3b>(A)= Vec3b(vb+(255-vb)*(A-128)/128,
                                    vg+(255-vg)*(A-128)/128,
                                    vr+(255-vr)*(A-128)/128);
        }
    }

    Mat res;
    //Transformamos la imagen gris al color deseado
    LUT(gris,lut,res);
    crear_nueva(nres,res);

}

//---------------------------------------------------------------------------
void ver_pinchar_estirar(int nfoto, int cx, int cy, double radio, double grado,bool guardar)
{
    //Copiamos la imagen de forma que ahora tengamos datos de tipo real.
    Mat S(foto[nfoto].img.rows,foto[nfoto].img.cols,CV_32FC1); //CV_32FC1 tipo de dato de la gausiana es un tipo de datos real de un canal

    for(int y=0; y<S.rows;y++){
        for(int x=0;x<S.cols;x++)
            //Cálculo de los valores de la superficie deformante
            S.at<float>(y,x)= exp(-((x-cx)*(x-cx)+(y-cy)*(y-cy))/(radio*radio));

    Mat Gx,Gy;
    //Calculamos la derivada en X
    Sobel(S,Gx,CV_32F,1,0,3,grado,0,BORDER_REFLECT);
    //Calculamos la derivada en Y
    Sobel(S,Gy,CV_32F,0,1,3,grado,0,BORDER_REFLECT);
    //Multiplicamos la superficie por su derivada en X e Y
    multiply(S,Gx,Gx);
    multiply(S,Gy,Gy);

    //Por cada posición de la superficie sumamos los valores a las derivadas.
    for(int y=0; y<S.rows;y++)
        for(int x=0; x<S.cols;x++){
            Gx.at<float>(y,x)+=x;
            Gy.at<float>(y,x)+=y;
   }

    Mat res;
    remap(foto[nfoto].img,res,Gx,Gy,INTER_LINEAR, BORDER_REFLECT);
    imshow("Pinchar/estirar",res);
    if(guardar)
    {
        res.copyTo(foto[nfoto].img);
        mostrar(nfoto);
        foto[nfoto].modificada=true;

    }

}
}
//---------------------------------------------------------------------------


void ver_matiz_saturacion_luminosidad(int nfoto, int matiz, double satu, double lumi,bool guardar)
{


    Mat hls;
    //Transformamos la imagen de RGB a HLS
    cvtColor(foto[nfoto].img,hls,COLOR_BGR2HLS_FULL);
    Mat canales[3];
    split(hls,canales);

    //Realizamos las aplicaciones sobre los canales
    //Aumentamos la profundidad del canal de Matiz para evitar la saturación del canal
    //y le aplicamos la suma.
    canales[0].convertTo(canales[0],CV_16S,1,matiz);
    //Nos quedamos con el byte menos significativo.
    bitwise_and(canales[0],255,canales[0]);
    canales[0].convertTo(canales[0],CV_8U);
    //Aplicamos las transformaciones sobre los demás canales
    canales[1]*=lumi;
    canales[2]*=satu;
    //Reunificamos los canales de la imagen en uno solo
    merge(canales,3,hls);

    Mat res;
    //Transformamos la imagen de HLS a BGR
    cvtColor(hls,res,COLOR_HLS2BGR_FULL);
    imshow(foto[nfoto].nombre,res);
    if(guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada=true;
    }


}

//---------------------------------------------------------------------------

void ver_perfilado(int nfoto,int tam, double grado, bool guardar)
{

    Mat laplace;
    //Calculamos la Laplaciana
    Laplacian(foto[nfoto].img,laplace, CV_16S,tam,-grado,0,BORDER_REFLECT);
    Mat img16;
    //Aumentamos la profundidad de la imagen
    foto[nfoto].img.convertTo(img16,CV_16S);
    //Sumamos la imagen con la Laplaciana
    img16=img16+laplace;
    Mat res;
    //Devolvemos la imagen a la profundidad original
    img16.convertTo(res,CV_8U);
    imshow(foto[nfoto].nombre,res);
    if(guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada=true;

    }
}

//---------------------------------------------------------------------------

void ver_perspectiva(int nfoto1, int nfoto2, Point2f pt1[], Point2f pt2[],bool guardar)
{
    //Obtenemos la matriz para la realizar la transformación perspectiva
    Mat M= getPerspectiveTransform(pt1,pt2);

    Mat imres=foto[nfoto2].img.clone();
    //Aplicamos la transformación
    warpPerspective(foto[nfoto1].img,imres,M,imres.size(), INTER_LINEAR,BORDER_TRANSPARENT);

    if(guardar){
        imres.copyTo(foto[nfoto2].img);
        foto[nfoto2].modificada=true;
        mostrar(nfoto2);
    }
    else{
        for(int i=0;i<4;i++)
        {
            line(imres,pt2[i],pt2[(i+1)%4],CV_RGB(0,0,0),2);
        }
        for(int i=0;i<4;i++)
        {
            circle(imres,pt2[i], 8, CV_RGB(0,255,0),-1);
        }
        imshow("Perspectiva",imres);
    }



}


//TAREAS OPCIONALES//
void ver_color_falso(int nfoto,int tipo_color, bool guardar){
    Mat img=foto[nfoto].img.clone();
    Mat img_color;
    //Aplicamos el color falso que se haya introducido
    applyColorMap(img,img_color,tipo_color);

    imshow(foto[nfoto].nombre,img_color);

    if (guardar) {
        img_color.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
        mostrar(nfoto);

    }
}

//---------------------------------------------------------------------------

void ver_rojo_verde_azul(int nfoto, double valores_mult[], double valores_suma[],bool guardar){

    Mat imres=foto[nfoto].img.clone();
    cvtColor(imres,imres,COLOR_BGR2RGB);

    Mat canales[3];
    split(imres,canales);

    canales[0]+=valores_suma[0];
    canales[1]+=valores_suma[1];
    canales[2]+=valores_suma[2];

    canales[0]*=valores_mult[0];
    canales[1]*=valores_mult[1];
    canales[2]*=valores_mult[2];



    merge(canales,3,imres);
    cvtColor(imres,imres,COLOR_RGB2BGR);
    imshow(foto[nfoto].nombre,imres);
    if (guardar) {
        imres.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
        mostrar(nfoto);

    }
}

//---------------------------------------------------------------------------

void ver_ecualizacion_histograma(int nfoto, int modo, bool guardar){

    Mat res;
    if(modo==0){
    Mat img= foto[nfoto].img;
    Mat gris, hist;
    cvtColor(img, gris, COLOR_BGR2GRAY);
    int canales[1]= {0}, bins[1]= {256};
    float rango[2]= {0, 256};
    const float *rangos[]= {rango};
    calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);
    hist*= 255.0/norm(hist, NORM_L1);
    Mat lut(1, 256, CV_8UC1);
    float acum= 0.0;
    for (int i= 0; i<256; i++) {
        lut.at<uchar>(0, i)= acum;
        acum+= hist.at<float>(i);
    }
    LUT(img, lut, res);
    imshow(foto[nfoto].nombre, res);
    }

        else{
    Mat img=foto[nfoto].img;
    Mat canales[3];
    split(img,canales);
    equalizeHist( canales[0],canales[0] );
    equalizeHist( canales[1],canales[1] );
    equalizeHist( canales[2],canales[2] );
    merge(canales,3,res);
    imshow(foto[nfoto].nombre,res);

    }

        if (guardar) {
            res.copyTo(foto[nfoto].img);
            foto[nfoto].modificada= true;
            mostrar(nfoto);

        }

}

//---------------------------------------
QString ver_informacion_imagen(int nfoto, int tipo){

    Mat imagen= foto[nfoto].img;

    QString valor;
    Scalar media_color = mean(imagen);
    int canales = imagen.channels();

    switch(tipo){
    case 0:
    {
        //tamaño (dimensiones)
        valor = QString("Anchura: %1, Altura: %2").arg(imagen.cols).arg(imagen.rows);
        break;
    }
    case 1:
    {
        //número de canales
        valor = QString::number(canales);
        break;
    }
    case 2:
    {
        //memoria ocupada
        int tamanioBytes = 0;
        if(imagen.isContinuous()){ // comprobamos si la matriz guarda sus elementos contiguos en memoria
            tamanioBytes = imagen.total() * imagen.elemSize();
        }else{
            tamanioBytes = imagen.step[0] * imagen.rows;
        }
        valor = QString("Tamaño en Bytes: %1").arg(tamanioBytes);
        break;
    }

    case 3:
    {
        //profundidad
        int profundidad = imagen.depth();
        switch(profundidad){
        //{ CV_8U=0, CV_8S=1, CV_16U=2, CV_16S=3, CV_32S=4, CV_32F=5, CV_64F=6 }
        case 0:
            valor = "CV_8U";
            break;
        case 1:
            valor = "CV_8S";
            break;
        case 2:
            valor = "CV_16U";
            break;
        case 3:
            valor = "CV_16S";
            break;
        case 4:
            valor = "CV_32S";
            break;
        case 5:
            valor = "CV_32F";
            break;
        case 6:
            valor = "CV_64F";
            break;
        }
        break;
    }
    case 4:{
        //ratio
        valor = QString::number(imagen.size().aspectRatio());
        break;
    }
    case 5: {
        //comprobamos el numero de canales y el tipo de datos de la imagen siendo este RGB (CV_8UC3)
        if (canales == 3 && imagen.type() == CV_8UC3) {
                   double media_imagen = (media_color[0] + media_color[1] + media_color[2]) / 3;
                   //BGR
                   double mediaRojo = media_color[2];
                   double mediaVerde = media_color[1];
                   double mediaAzul = media_color[0]; //B

                   valor = QString("Color medio de la imagen: %1 \nColor Rojo: %2 \nColor Verde: %3 \nColor Azul: %4 ")
                           .arg(media_imagen).arg(mediaRojo).arg(mediaVerde).arg(mediaAzul);
        } else if (canales == 1){
                valor = QString("Color medio de la imagen: %1 ").arg(media_color[0]);
        }

        break;
    }
    case 6:{

        if(canales == 3 && imagen.type() == CV_8UC3){
        //estilo color
           QColor color = QColor(media_color[2],media_color[1],media_color[0]);
           valor= "background-color: rgb(";
           valor+= QString::number(color.red())+",";
           valor+= QString::number(color.green())+",";
           valor+= QString::number(color.blue())+")";

        } else if (canales == 1) {            
            //valor= "background-color: gray(";
            //valor+= QString::number((int)round(media_color[0]))+")";
            QColor color = QColor(media_color[0],media_color[0],media_color[0]);
            valor= "background-color: rgb(";
            valor+= QString::number(color.red())+",";
            valor+= QString::number(color.green())+",";
            valor+= QString::number(color.blue())+")";
        }
    }
        break;
    }




      return valor;

}

//---------------------------------------------------------------------------

void cambiar_modelo_color(int nfoto, int formato, bool guardar){
    Mat imagen = foto[nfoto].img;
    Mat res;

    //Cambiamos del modelo BGR a los distintos tipos
    switch(formato){
    case 0:
        //HLS
        cvtColor(imagen,res,COLOR_BGR2HLS);
        break;

    case 1:
        //HSV
        cvtColor(imagen,res,COLOR_BGR2HSV);
        break;

    case 2:
        //XYZ
        cvtColor(imagen,res,COLOR_BGR2XYZ);
        break;

     case 3:
        //YUV
        cvtColor(imagen,res,COLOR_BGR2YUV);
        break;
     case 4:
        //GRAY
        cvtColor(imagen,res,COLOR_BGR2GRAY);
        break;
    }

    if(guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
        mostrar(nfoto);
    }
    else{
        imshow("Modelo Color",res);
    }
}

//-------------------------------------------------------
void ver_histograma_bidimensional (int nfoto, int nres, int tipo, int celdas, bool guardar){
    Mat imagen = foto[nfoto].img;
    Mat hist;
    int canales[2];
    switch (tipo)
    {
    case 0:{
        canales[0] = 2; //R
        canales[1] = 1; //G
        break;
    }
    case 1:{
        canales[0] = 2; //R
        canales[1] = 0; //B
        break;
    }
    case 2:{
        canales[0] = 1; //G
        canales[1] = 0; //B
        break;
    }
    }

    int bins[2]= {celdas, celdas};
    float rango[2]= {0, 256};
    const float *rangos[]= {rango, rango};
    calcHist(&imagen, 1, canales, noArray(), hist, 2, bins, rangos);

    // Operaciones para pintar el histograma

    Mat pinta(celdas, celdas, CV_8UC1);
    double minval, maxval;
    minMaxLoc(hist, &minval, &maxval); // Para escalar el color entre blanco y negro

    for (int r= 0; r<celdas; r++)
        for (int g= 0; g<celdas; g++)
            pinta.at<uchar>(r, g)= 255-255*hist.at<float>(r, g)/maxval;

    if (guardar){
      crear_nueva(nres, pinta);
     } else {
      imshow("Histograma Bidimensional", pinta);
     }
}


string Lt1(string cadena)
{
    QString temp= QString::fromUtf8(cadena.c_str());
    return temp.toLatin1().data();
}

//---------------------------------------------------------------------------
