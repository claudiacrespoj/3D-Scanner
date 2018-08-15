#include "window.h"
#include "ui_window.h"
using namespace std;

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    port= new QSerialPort("COM6"); //we create the port
    port->open(QIODevice::ReadWrite ); //we open the port
    ui->pushButton_2->setEnabled(false);
    if(port->isOpen()){
        openSerialPort();
        QMessageBox::information(this,"Informacion","El dispositivo esta Conectado");
    }
    else {
        QMessageBox::warning(this, "Error", "¡No hay dispositivo conectado!");
        ui->pushButton->setEnabled(false);
    }
}

Window::~Window()
{
    delete ui;
}
/******************************     ABRIR PUERTO DEL ARDUINO        ****************************/
void Window::openSerialPort(){
    port->setBaudRate(9600);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setDataBits(QSerialPort::Data8);
    port->setStopBits(QSerialPort::OneStop);
}
/**************************     ESCRIBIR EN EL PUERTO, MOVER MOTOR        ************************/
void Window::writeSerialPort(short forma){
    char *a;
    if(forma==1) a="f";
    if(forma==2) a="+";
    if(forma==3) a="c";
    if(forma==4) a="-";
    port->clear();
    port->write(a);
    port->flush();

}
/******************************          INICIAR ESCANEO         ****************************/
void Window::on_pushButton_clicked()
{
    stopScan=false;
    CvCapture* capture =cvCreateCameraCapture(0);   //Creo un objeto para capturar con Cámaras
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,1280);     //Ancho de la imagen
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,720);     //Alto de la imagen
    scanShape(capture);                 //ESCANEO DE FORMA
    if(stopScan){                       //verifica si el objeto es inválido
        stopScan=false;
        QMessageBox::warning(this,tr("Advertencia"),tr("Objeto invalido"));
        cvReleaseCapture(&capture);
        cvDestroyAllWindows();
        resetObj();
        return;
    }
    cvWaitKey(2000);
    scanColor(capture);
    for(int i=0;i<200;i++){
        for(int j=0;j<60;j++){
            perfil[i].shape[j].x=Perfil[i].cilindro[j].x;
            perfil[i].shape[j].y=Perfil[i].cilindro[j].y;
            perfil[i].shape[j].z=Perfil[i].cilindro[j].z;
            Perfil[i].cilindro[j].x=0;
            Perfil[i].cilindro[j].y=0;
            Perfil[i].cilindro[j].z=0;
            perfil[i].color[j].b=Perfil[i].colores[j].val[0];
            perfil[i].color[j].g=Perfil[i].colores[j].val[1];
            perfil[i].color[j].r=Perfil[i].colores[j].val[2];
            perfil[i].color[j].a=Perfil[i].colores[j].val[3];
            Perfil[i].colores[j].val[0]=0;
            Perfil[i].colores[j].val[1]=0;
            Perfil[i].colores[j].val[2]=0;
            Perfil[i].colores[j].val[3]=0;
        }
        Perfil[i].numPts=0;
        cvSetZero(Perfil[i].colorData);
    }
    ui->pushButton_2->setEnabled(true);
    cvReleaseCapture(&capture);
    cvDestroyAllWindows();
}

/**************************     ESCANEO DE FORMA     *************************************/
void Window::scanShape(CvCapture *capture){
    writeSerialPort(1);
    cvWaitKey(2000);
    IplImage *frame;
    QProgressDialog verForma("Escaneando forma...","Cancel",0,200,this);
    for(int i=0;i<200;i++){
        int X=0;
        while(1) {
            frame = cvQueryFrame(capture);  //captura foto con la cámara
            cvWaitKey(33);                  //Retardo tiempo para cámaras de 30FPS
            if(i==0){                       //Se espera 2seg para encender la cámara
                X++;
                if(X>=60) break;            //Cámara encendida, primera foto
            }
            else break;                     //Cámara encendida, fotos 2 a 200
        }
        Perfil[i].minimalPts=false;         //Parámetro para verificar los puntos del perfil
        Perfil[i].Scan3D(frame,i);          //Procesamiento del perfil
        cout<<"\n\tFOTO >>>  "<<i;
        if(Perfil[i].minimalPts) {          //Verifica los puntos del perfil
            Perfil[i].minimalPts=false;
            stopScan=true;
            errorPos=i;
            return;                         //Detiene el escaneo si el No. de puntos es menor al mínimo
        }
        verForma.setValue(i);
        writeSerialPort(2);
        cvWaitKey(700);                     //Retardo de 500 ms
    }
    writeSerialPort(3);
    cvWaitKey(2000);
    verForma.setValue(200);
}

/**************************     ESCANEO DE COLOR     *************************************/

void Window::scanColor(CvCapture *capture){
    QProgressDialog verColor("Escaneando color...","Cancel",0,200,this);
    IplImage* frame;
    for(int i=0;i<200;i++){
        cvWaitKey(33);                  //Retardo tiempo para cámaras de 30FPS
        frame = cvQueryFrame(capture);  //Captura foto de la cámara
        cvShowImage("Escaneo Color",frame);  //Muestra en pantalla la foto capturada
        for(int j=0;j<60;j++){          //Extrae los datos de color del perfil
            int x=cvGetReal2D(Perfil[i].colorData,j,0);
            int y=cvGetReal2D(Perfil[i].colorData,j,1);
            Perfil[i].colores[j]=cvGet2D(frame,x,y);    //Se almacenan los datos de color
        }
        verColor.setValue(i);
        writeSerialPort(4);
        cvWaitKey(700);
    }
    verColor.setValue(200);
}

void Window::resetObj(){
    if(errorPos==0) return;
    else{
        for(int i=0;i<errorPos-1;i++){
            for(int j=0;j<60;j++){
                Perfil[i].cilindro[j].x=0;
                Perfil[i].cilindro[j].y=0;
                Perfil[i].cilindro[j].z=0;
            }
            Perfil[i].numPts=0;
            cvSetZero(Perfil[i].colorData);
        }
    }
}

void Window::on_pushButton_2_clicked()
{
    QString fileName=QFileDialog::getSaveFileName(this,tr("Guardar Archivo"),"C://",tr("All files (*)"));
    if (fileName.isEmpty()) return;
    std::string nomStr = fileName.toUtf8().constData();
    char *nombre = new char[nomStr.length() + 1];
    strcpy(nombre, nomStr.c_str());
    ofstream out(nombre, ios::out | ios::binary);
    out.write((char *) &perfil, sizeof perfil);
    out.close();
}
