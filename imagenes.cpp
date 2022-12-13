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
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    //Si no hay difuminación se escribe directamente en la imagen.
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, color_pincel, -1, LINE_AA);
    else {
        //mejorado para que en lugar de cogr la foto entera, tan solo coja un roi pequeño y funcione mejor
        int tam= radio_pincel+difum_pincel;
        int posx= tam, posy=tam;
        Rect roi(x-tam,y-tam,2*tam+1,2*tam+1);
        if(roi.x<0){
            //decrementamos del roi lo que se salga
            roi.width+=roi.x;
            posx+=roi.x;
            roi.x=0;
        }
        if(roi.y<0) {
               roi.height+=roi.y;
               posy+=roi.y;
               roi.y=0;
        }
        //nos salimos por la derecha
        if(roi.x+roi.width > im.cols){
            roi.width=im.cols-roi.x;
        }
        if(roi.y+roi.height > im.rows){
            roi.height=im.rows-roi.y;
        }
        Mat frag=im(roi);
        Mat res(frag.size(), frag.type(), color_pincel);
        Mat cop(frag.size(), frag.type(), CV_RGB(0,0,0));
        circle(cop, Point(tam, tam), radio_pincel, CV_RGB(255,255,255), -1, LINE_AA);
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


Scalar ColorArcoIris ()
{
    //COMPLETAR CON LO QUE HA HECHO EN CLASE
    static Scalar colorActual=CV_RGB(255,0,0);
    //Automata que comprueba el estado en el que estamos dependiendo del color
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
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    //Si no hay difuminación se escribe directamente en la imagen.
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, ColorArcoIris(), -1, LINE_AA);
    else {
        //mejorado para que en lugar de cogr la foto entera, tan solo coja un roi pequeño y funcione mejor
        int tam= radio_pincel+difum_pincel;
        int posx= tam, posy=tam;
        Rect roi(x-tam,y-tam,2*tam+1,2*tam+1);
        if(roi.x<0){
            //decrementamos del roi lo que se salga
            roi.width+=roi.x;
            posx+=roi.x;
            roi.x=0;
        }
        if(roi.y<0) {
               roi.height+=roi.y;
               posy+=roi.y;
               roi.y=0;
        }
        //nos salimos por la derecha
        if(roi.x+roi.width > im.cols){
            roi.width=im.cols-roi.x;
        }
        if(roi.y+roi.height > im.rows){
            roi.height=im.rows-roi.y;
        }
        Mat frag=im(roi);
        Mat res(frag.size(), frag.type(), ColorArcoIris());
        Mat cop(frag.size(), frag.type(), CV_RGB(0,0,0));
        circle(cop, Point(tam, tam), radio_pincel, CV_RGB(255,255,255), -1, LINE_AA);
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
    //tratatmiento gama
    Mat img32f;
    //convertimos la imagen que tenemos a 32f
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
    //Si no se aplica el filtro blur
    else if (ntipo == 2)
        blur(fragmento, fragmento, Size(tamx, tamy));
    else if (ntipo==3)
        //aplicamos un limite al suavizado para que no pete. El segundo valor esta puesto un poco al azar.
        medianBlur(fragmento,fragmento,min(tamx,301));
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
    if(canal==3){
    calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);
    } else{
        //TERMINAR
        calcHist(&(foto[nfoto].img), 1, &canal, noArray(), hist, 1, bins, rangos);
    }
    minMaxLoc(hist, &vmin, &vmax);

    for (int i= 0; i<256; i++){
        float poshist=185-hist.at<float>(i)/vmax*182;
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


    //reescalamos la imagen del fondo para que lo ocupe entero

    //dependiendo del parametro fondo se va escogiendo una foto u otra
    QImage imq= QImage(nombres[fondo]);
    Mat imgfondo(imq.height(),imq.width(),CV_8UC4,imq.scanLine(0));
    cvtColor(imgfondo, imgfondo, COLOR_RGBA2RGB);
    resize(imgfondo,imgfondo,foto[nfoto].img.size());

    //Convertimos la imagen a gris, le pasamos el filtro de sobel
    Mat gris; //imagen original
    cvtColor(foto[nfoto].img, gris,COLOR_BGR2GRAY);
    Mat rotada;
    rotar_angulo(gris,rotada,angulo,1.0,1); //rotamos un trozo de la imagen sobre la original??
    Mat sobel;
    //tenemos que
    Sobel(rotada,sobel,CV_8U,1,0,3,grado, 128);
    rotar_angulo(sobel, rotada,-angulo,1.0,0);
    gris= rotada(Rect((rotada.cols-gris.cols)/2,(rotada.rows-gris.rows)/2,gris.cols,gris.rows));


    Mat array[3]={gris,gris,gris};
    Mat res;
    merge(array,3,res);
    addWeighted(imgfondo,1.0,res,1.0,-128, res);

    imshow("Bajorrelieve",res);
    if(guardar){
        crear_nueva(primera_libre(),res);
    }
    else{
        imshow("Bajorrelieve",res);
    }

}

//---------------------------------------------------------------------------
void ver_ajuste_lineal(int nfoto, double pmin, double pmax, bool guardar){

//Cogemos la imagen, la convertimos a gris, calculamos el histograma, calculamos los percentiles y luego
//hacemos la operación de estiramiento lineal.
    Mat gris;
    cvtColor(foto[nfoto].img,gris,COLOR_BGR2GRAY);
    int canales[1]={0};
    int bins[1]={256};
    float rango[2]={0,256};
    const float *rangos[]={rango};
    Mat hist;
    calcHist(&gris,1,canales,noArray(),hist,1,bins,rangos);


    //escala el histograma de forma que los valores del histograma sumen en total 100
    normalize(hist,hist, 100, 0, NORM_L1);

    double acum=0;
    int vmin=0;

    for(;vmin<256 && acum<pmin;vmin++)
        acum+=hist.at<float>(vmin);

    acum=0;

    //cuando acum llega al percentil, entonces se para creo. vmax almacena el valor máximo en el lado derecho
    //obteniendo el pixel que se encuentra justo en el percentil indicado o algo de eso.
    int vmax=255;
    for(; vmax>=0 && acum<pmax; vmax--)
        acum+=hist.at<float>(vmax);

    //condiciones para evitar que al estirar el histograma haya problemas. Vmin tiene que ser menor que vmax
    if(vmin>=vmax) vmax=vmin+1;
    double a= 255.0/(vmax-vmin);
    double b= -vmin*a;

    Mat res;
    foto[nfoto].img.convertTo(res,CV_8U,a,b);
    imshow(foto[nfoto].nombre,res);
    if(guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada=true;
    }

}

//---------------------------------------------------------------------------

void escala_color(int nfoto, int nres){
    //MEJORA: fijar la parte donde se corta segun el valor de gris del color objetivo.

    Mat gris;
    cvtColor(foto[nfoto].img,gris,COLOR_BGR2GRAY);
    //pasamos la imagen gris a "BGR" para crear los canales de color
    cvtColor(gris,gris,COLOR_GRAY2BGR);

    //creamos la tabla LUT con tres canales de 8 bits cada uno.
    Mat lut(1,256,CV_8UC3);
    int vb=color_pincel.val[0]; //color azul del pincel
    int vg=color_pincel.val[1];
    int vr=color_pincel.val[2];
    for(int A=0;A<256;A++){
        //Cada posición de la tabla tendrá los tres valores
        if(A<128){
            //aqui estamos modificando el color azul
            lut.at<Vec3b>(A)= Vec3b(vb*A/128, vg*A/128, vg*A/128);
        }
        else {
            lut.at<Vec3b>(A)= Vec3b(vb+(255-vb)*(A-128)/128,
                                    vg+(255-vg)*(A-128)/128,
                                    vr+(255-vr)*(A-128)/128);
        }
    }

    Mat res;
    LUT(gris,lut,res);
    //creamos en la posición nres la imagen res
    crear_nueva(nres,res);

}

//---------------------------------------------------------------------------
void ver_pinchar_estirar(int nfoto, int cx, int cy, double radio, double grado,bool guardar)
{
    //La imagen de la Gaussiana tiene las mismas dimensiones que la imagen que coge.
    //Su tipo de datos ahora será real
    Mat S(foto[nfoto].img.rows,foto[nfoto].img.cols,CV_32FC1);
    for(int y=0; y<S.rows;y++){
        for(int x=0;x<S.cols;x++)
            S.at<float>(y,x)= exp(-((x-cx)*(x-cx)+(y-cy)*(y-cy))/(radio*radio));
    Mat Gx,Gy;
    Sobel(S,Gx,CV_32F,1,0,3,grado,0,BORDER_REFLECT);
    Sobel(S,Gy,CV_32F,0,1,3,grado,0,BORDER_REFLECT);
    multiply(S,Gx,Gx);
    multiply(S,Gy,Gy);

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
    //Separaremos la imagen de tres canales en tres imagenes de un canal, mediante split.Una vez dividido en los canales
    // HSL, aplicaremos un filtro diferente a cada uno de ellos. Finalmente, volveremos a juntarlos mediante Merge.
    // Finalmente la reconvertiremos de HSL a RGB

    Mat hls;
    cvtColor(foto[nfoto].img,hls,COLOR_BGR2HLS_FULL); //HSL es un formato que utiliza grados. Mediante full lo convertimos a 255
    Mat canales[3];
    split(hls,canales);

    //Realizamos las aplicaciones sobre los canales
    canales[0].convertTo(canales[0],CV_16S,1,matiz); //hacemos una conversión de profundidad para evitar saturacion?
    bitwise_and(canales[0],255,canales[0]);
    canales[0].convertTo(canales[0],CV_8U);
    canales[1]*=lumi;
    canales[2]*=satu;
    merge(canales,3,hls);

    Mat res;
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

    //Revisar
    Mat laplace;
    Laplacian(foto[nfoto].img,laplace, CV_16S,tam,-grado,0,BORDER_REFLECT);
    Mat img16;
    foto[nfoto].img.convertTo(img16,CV_16S);
    img16=img16+laplace;
    Mat res;
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
    Mat M= getPerspectiveTransform(pt1,pt2); //matriz que define la transformacion

    //Vamos a clonar la nfoto2 para el tema de la previsualizacion, pintando sobre ella.
    Mat imres=foto[nfoto2].img.clone();
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

    }
    imshow("Perspectiva",imres);



}


//TAREAS OPCIONALES//
void ver_color_falso(int nfoto,int tipo_color, bool guardar){
    Mat img=foto[nfoto].img.clone();
    Mat img_color;
    applyColorMap(img,img_color,tipo_color);
    imshow("Resultado",img_color);

    if(guardar){
        destroyWindow("Resultado");
        crear_nueva(primera_libre(),img_color);
    }
}


string Lt1(string cadena)
{
    QString temp= QString::fromUtf8(cadena.c_str());
    return temp.toLatin1().data();
}

//---------------------------------------------------------------------------