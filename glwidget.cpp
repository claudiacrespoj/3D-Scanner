#include <QtWidgets>
#include <QtOpenGL>
#include "glwidget.h"
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif
using namespace std;
//int Xangle=0,Yangle=0,Zangle=0;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    Xangle=0;
    Yangle=0;
    Zangle=0;
    fileChar=false;
    ptBezGL=20;
    R=0.0;
    G=0.0;
    B=1.0;
    shapeCheck=0;
}

GLWidget::~GLWidget(){}
/********************************     CHANGING SIZE CONFIGURATION  *************************************/
QSize GLWidget::minimumSizeHint() const
 {
     return QSize(350, 350);
 }

 QSize GLWidget::sizeHint() const
 {
     return QSize(640, 480);
 }

 static void qNormalizeAngle(int &angle)
 {
     while (angle < -180)
         angle += 180;
     while (angle > 180)
         angle -= 180;
 }


/********************************     CHANGING ANGLES CONFIGURATION   *************************************/

void GLWidget::angleDefault(){
    Xangle=-150;
    Yangle=0;
    Zangle=0;
    emit signalX(Xangle);
    emit signalY(Yangle);
    emit signalZ(Zangle);
    update();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<     CHANGE ANGLES X
void GLWidget::setXangle(int angle){
    qNormalizeAngle(angle);
    if(angle!=Xangle){
        Xangle=angle;
        //emit Xchanged(angle);
        update();
    }
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<     CHANGE ANGLES Y
void GLWidget::setYangle(int angle){
    qNormalizeAngle(angle);
    if(angle!=Yangle){
        Yangle=angle;
        //emit Ychanged(angle);
        update();
    }
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<     CHANGE ANGLES Z
void GLWidget::setZangle(int angle){
    qNormalizeAngle(angle);
    if(angle!=Zangle){
        Zangle=angle;
        //emit Zchanged(angle);
        update();
    }
}

/********************************    CHANGE COLOR CONFIGURATION   *************************************/

void GLWidget::initColorGL(){
    float aR=0.4*R,aG=0.4*G,aB=0.4*B;
    float mR=0.7*R,mG=0.7*G,mB=0.7*B;
    float ambient[] = {aR, aG, aB, 1.0};
    float position0[] = {-40.0, 0.0, 50.0, 1.0};
    float position1[] = {40.0, 0.0, -50.0, 1.0};
    float mat_diffuse[] = {mR,mG, mB, 1.0};
    float mat_specular[] = {R, G, B, 1.0};
    float mat_shininess[] = {50.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}


void GLWidget::changeObjCol(int estilo){
    if(estilo==0){
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHTING);
    }
    else{
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
        initColorGL();
    }
    updateGL();
}

void GLWidget::changeScreen(int fondo){
    if(fondo==0) glClearColor(1.0,1.0,1.0,1);
    else glClearColor(0.0,0.0,0.0,1);
    updateGL();
}

void GLWidget::rojo(double redGL){
    R=float(redGL);
    initColorGL();
    updateGL();
}

void GLWidget::verde(double greenGL){
    G=float(greenGL);
    initColorGL();
    updateGL();
}

void GLWidget::azul(double blueGL){
    B=float(blueGL);
    initColorGL();
    updateGL();
}

void GLWidget::initializeGL(){
    glClearColor(1.0,1.0,1.0,1);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);
}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(GLfloat(Xangle),1.0,0.0,0.0);
    glRotatef(GLfloat(Yangle),0.0,1.0,0.0);
    glRotatef(GLfloat(Zangle),0.0,0.0,1.0);
    if(fileChar){
        grafica2();
        grafica1(1);
        grafica1(0);
    }
    else{
        glBegin(GL_TRIANGLES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(-50.0,-50.0,0.0);
        glColor3f(0.0,1.0,0.0);
        glVertex3f(0.0,50.0,0.0);
        glColor3f(0.0,0.0,1.0);
        glVertex3f(50.0,-50.0,0.0);
        glEnd();
    }
}

void GLWidget::resizeGL(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
          glOrtho(-70.0, 70.0, -70.0*(GLfloat)h/(GLfloat)w,
                  70.0*(GLfloat)h/(GLfloat)w, -70.0,70.0);
       else
          glOrtho(-70.0*(GLfloat)w/(GLfloat)h,
                  70.0*(GLfloat)w/(GLfloat)h,-70.0,70.0,-70.0,70.0);
       glMatrixMode(GL_MODELVIEW);
}
/********************************       GRAPHICS    ********************************************************/
void GLWidget::grafica1(int m){
    float X,Y,Z,R,G,B,puntos[21][21][3],color[21][21][4];
    float midZ=0;//mm2px=3.9594;
    //X=new float;Y=new float;Z=new float;R=new float;G=new float;B=new float;
    int step2=m*20,step=0;
    for(int k=0;k<10;k++){
        for(int i=0;i<21;i++){
            for(int j=0;j<21;j++){
                if(k==9 && j==20){
                    midZ=(perfil[0].shape[59].z-perfil[0].shape[0].z)/2.0;
                    R=perfil[0].color[i+step2].r/255.0;
                    G=perfil[0].color[i+step2].g/255.0;
                    B=perfil[0].color[i+step2].b/255.0;
                    X=perfil[0].shape[i+step2].x;
                    Y=perfil[0].shape[i+step2].y;
                    Z=perfil[0].shape[i+step2].z-midZ;
                }
                else {
                    midZ=(perfil[j+step].shape[59].z-perfil[j+step].shape[0].z)/2.0;
                    R=perfil[j+step].color[i+step2].r/255.0;
                    G=perfil[j+step].color[i+step2].g/255.0;
                    B=perfil[j+step].color[i+step2].b/255.0;
                    X=perfil[j+step].shape[i+step2].x;
                    Y=perfil[j+step].shape[i+step2].y;
                    Z=perfil[j+step].shape[i+step2].z-midZ;
                }
                puntos[i][j][0]=X/2.0;
                puntos[i][j][1]=Z;
                puntos[i][j][2]=Y/2.0;
                color[i][j][0]=R;
                color[i][j][1]=G;
                color[i][j][2]=B;
                color[i][j][3]=1.0;
                //std::cout<<"\n\tbezier "<<*X<<"  "<<*Y<<"  "<<*Z;
            }
        }
        glMap2f(GL_MAP2_VERTEX_3,0.0,1.0,3,21,0.0,1.0,63,21,&puntos[0][0][0]);
        glMap2f(GL_MAP2_COLOR_4,0.0,1.0,4,21,0.0,1.0,84,21,&color[0][0][0]);
        glEnable(GL_MAP2_VERTEX_3);
        glEnable(GL_MAP2_COLOR_4);
        glMapGrid2f(ptBezGL,0.0,1.0,ptBezGL,0.0,1.0);
        if(shapeCheck==0) glEvalMesh2(GL_FILL,0,ptBezGL,0,ptBezGL);
        if(shapeCheck==1) glEvalMesh2(GL_LINE,0,ptBezGL,0,ptBezGL);
        if(shapeCheck==2) glEvalMesh2(GL_POINT,0,ptBezGL,0,ptBezGL);
        step+=20;
    }
    glFlush();
    //delete X;delete Y;delete Z;delete R;delete G;delete B;
}

void GLWidget::grafica2(){
    float X,Y,Z,R,G,B,puntos[20][21][3],color[20][21][4];
    float midZ=0;//mm2px=3.9594;
    //X=new float;Y=new float;Z=new float;R=new float;G=new float;B=new float;
    int step2=40,step=0;
    for(int k=0;k<10;k++){
        for(int i=0;i<20;i++){
            for(int j=0;j<21;j++){
                if(k==9 && j==20){
                    midZ=(perfil[0].shape[59].z-perfil[0].shape[0].z)/2.0;
                    R=perfil[0].color[i+step2].r/255.0;
                    G=perfil[0].color[i+step2].g/255.0;
                    B=perfil[0].color[i+step2].b/255.0;
                    X=perfil[0].shape[i+step2].x;
                    Y=perfil[0].shape[i+step2].y;
                    Z=perfil[0].shape[i+step2].z-midZ;
                }
                else {
                    midZ=(perfil[j+step].shape[59].z-perfil[j+step].shape[0].z)/2.0;
                    R=perfil[j+step].color[i+step2].r/255.0;
                    G=perfil[j+step].color[i+step2].g/255.0;
                    B=perfil[j+step].color[i+step2].b/255.0;
                    X=perfil[j+step].shape[i+step2].x;
                    Y=perfil[j+step].shape[i+step2].y;
                    Z=perfil[j+step].shape[i+step2].z-midZ;
                }
                puntos[i][j][0]=X/2.0;
                puntos[i][j][1]=Z;
                puntos[i][j][2]=Y/2.0;
                color[i][j][0]=R;
                color[i][j][1]=G;
                color[i][j][2]=B;
                color[i][j][3]=1.0;
            }
        }
        glMap2f(GL_MAP2_VERTEX_3,0.0,1.0,3,21,0.0,1.0,63,20,&puntos[0][0][0]);
        glMap2f(GL_MAP2_COLOR_4,0.0,1.0,4,21,0.0,1.0,84,20,&color[0][0][0]);
        glEnable(GL_MAP2_VERTEX_3);
        glEnable(GL_MAP2_COLOR_4);
        glMapGrid2f(ptBezGL,0.0,1.0,ptBezGL,0.0,1.0);
        if(shapeCheck==0) glEvalMesh2(GL_FILL,0,ptBezGL,0,ptBezGL);
        if(shapeCheck==1) glEvalMesh2(GL_LINE,0,ptBezGL,0,ptBezGL);
        if(shapeCheck==2) glEvalMesh2(GL_POINT,0,ptBezGL,0,ptBezGL);
        step+=20;
    }
    glFlush();
    //delete X;delete Y;delete Z;delete R;delete G;delete B;
}

void GLWidget::numPointGL(int nPts){
    ptBezGL=nPts;
    updateGL();
}

void GLWidget::solidCheck(bool estado){
    if(estado){
        glEnable(GL_SMOOTH);
        shapeCheck=0;
        updateGL();
    }
}

void GLWidget::wireCheck(bool estado){
    if(estado){
        glEnable(GL_FLAT);
        shapeCheck=1;
        updateGL();
    }
}

void GLWidget::pointCheck(bool estado){
    if(estado){
        glEnable(GL_FLAT);
        shapeCheck=2;
        updateGL();
    }
}

/********************************     mouse entry    *************************************/
void GLWidget::mousePressEvent(QMouseEvent *event)
 {
    lastPos=event->pos();
 }

void GLWidget::mouseMoveEvent(QMouseEvent *event)
 {
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton) {
         setXangle(Xangle + dy);
         setYangle(Yangle + dx);
     } else if (event->buttons() & Qt::RightButton) {
         setXangle(Xangle + dy);
         setZangle(Zangle + dx);
     }
     lastPos = event->pos();
}
/*********************************    Load Archive        ********************************************/
void GLWidget::cargarArchivo(){
    QString fileName = QFileDialog::getOpenFileName(this,
             tr("Abrir Archivo"), "C://",
             tr("All Files (*)"));
    std::string nomStr = fileName.toUtf8().constData();
    char *nombre = new char[nomStr.length() + 1];
    strcpy(nombre, nomStr.c_str());
    for(int i=0;i<200;i++){
        for(int j=0;j<60;j++){
            perfil[i].color[j].r=0.0;
            perfil[i].color[j].g=0.0;
            perfil[i].color[j].b=0.0;
            perfil[i].color[j].a=0.0;
            perfil[i].shape[j].x=0.0;
            perfil[i].shape[j].y=0.0;
            perfil[i].shape[j].z=0.0;
        }
    }
    ifstream datoGL(nombre,ios::in|ios::binary);
    datoGL.read((char *) &perfil,sizeof perfil);
    datoGL.close();
    fileChar=true;
    updateGL();
}
