#ifndef S3DFUNCTIONS_H_INCLUDED
#define S3DFUNCTIONS_H_INCLUDED
#include <D:\OpencvQT\install\include\opencv/cv.h>
#include <D:\OpencvQT\install\include\opencv/cxcore.h>
#include <D:\OpencvQT\install\include\opencv/highgui.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

class ScanObj{
    private:
        IplImage* skeleton(IplImage* original){
            IplImage*  skel;
            skel= cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
            IplImage*  temp= cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
            IplImage*  eroded= cvCreateImage(cvGetSize(original),IPL_DEPTH_8U,1);
            IplConvKernel* element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_CROSS,NULL);
            bool done;
            do{
                    cvErode(original, eroded, element);
                    cvDilate(eroded, temp, element);
                    cvSub(original, temp, temp);
                    cvOr(skel, temp, skel);
                    cvCopy(eroded,original);
                    done = (cvNorm(original) == 0);
            }while (!done);
            return skel;
        }
/**********************************************************************************************************
***********************  Busca las posiciones de un punto con valor diferente de 0  ***********************
**********************************************************************************************************/
        CvMat* posFind(IplImage* inImg,int inRow){
            CvMat* outPos=cvCreateMat(inRow,2,CV_64FC1);
			cvSetZero(outPos);
			double valor;
			bool nonZero=FALSE;
            int x=0,posJ;
            for(int i=0;i<inImg->height;i++){
				for(int j=550;j<inImg->width;j++){
					valor=cvGetReal2D(inImg,i,j);
					if(valor!=0.0){
						posJ=j;
						nonZero=TRUE;
					}
				}
				if(nonZero==TRUE){
					cvSetReal2D(outPos,x,0,i);
					cvSetReal2D(outPos,x,1,posJ);
					x++;
				}
				nonZero=FALSE;
			}
			return outPos;
        }
/**********************************************************************************************************
*********************Funcion que transforma las coordenadas de pixeles a milimetros************************
**********************************************************************************************************/
        CvMat* pix2mm(CvMat* pixRow,CvMat* pixCol){
			double radCoef[]={7.79532557748263*pow(10.0,-7),-0.00109437105325829,0.611308781228893,-0.0695838747184494};
            double mm2pixCoef[]={6.004210653803*pow(10.0,-7),-0.00101156056201779,0.217442845277859};
            double X,Xmm,mm2pixFac,Ymm,pi=3.1415926535;
			CvMat* miliData=cvCreateMat(pixRow->rows,2,CV_64FC1);
			for(int i=0;i<pixCol->rows;i++){
                    X=cvGetReal1D(pixCol,i);
                    Xmm=(radCoef[0]*pow(X,3)+radCoef[1]*pow(X,2)+radCoef[2]*X+radCoef[3])/cos(0.1*pi);
                    mm2pixFac=mm2pixCoef[0]*pow(Xmm,2)+mm2pixCoef[1]*Xmm+mm2pixCoef[2];
                    Ymm=mm2pixFac*cvGetReal1D(pixRow,i);
                    cvSetReal2D(miliData,i,0,Xmm);
                    cvSetReal2D(miliData,i,1,Ymm);
            }
        return miliData;
        }

    public:
        int numPts;
        bool minimalPts;
        CvScalar colores[60];
        CvPoint3D32f cilindro[60];
        CvMat* colorData;
        void Scan3D(IplImage* colImg,int number){
			CvMat* newPosData;
            newPosData=cvCreateMat(60,2,CV_64FC1);
            colorData=cvCreateMat(60,2,CV_64FC1);
            IplImage* grayImg, *binImg,*proImg,*skImg;   //Declaracion de variables para almacenar imagenes"
            grayImg= cvCreateImage(cvGetSize(colImg),IPL_DEPTH_8U,1);   //Se inicializa variable con el mismo tamaño de la original
            cvCvtColor(colImg, grayImg, CV_BGR2GRAY);                   //La imagen es convertida a escala de grises
            binImg= cvCreateImage(cvGetSize(colImg),IPL_DEPTH_8U,1);    //Se inicializa variable para la imagen binaria
            proImg= cvCreateImage(cvGetSize(colImg),IPL_DEPTH_8U,1);    //Se inicializa variable para la imagen procesada
            //cvAdaptiveThreshold(grayImg,binImg,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY);
            cout<<"\n\tBINARIZACION";
            cvThreshold(grayImg,binImg,225,255,CV_THRESH_BINARY);       //Se binariza la imagen
            IplConvKernel* modMat=cvCreateStructuringElementEx(7,7,1,1,CV_SHAPE_ELLIPSE,NULL);  //Se crea una máscara
            cvMorphologyEx(binImg,proImg,NULL,modMat,CV_MOP_CLOSE,1);   //Se aplica una funcion CLOSE
            skImg= cvCreateImage(cvGetSize(colImg),IPL_DEPTH_8U,1);
            skImg=skeleton(proImg);                        //Se esqueletiza la imagen
            cvShowImage("dickbutt",skImg);
            //cout<<"\n\tESQUELETIZACION";
            int *tamano;
			tamano=new int (cvCountNonZero(skImg));              //Cuenta el numero pixeles con valores diferentes de 0
            if(*tamano>1000) numPts=999;
            else numPts=*tamano;
			delete tamano;
            CvMat* posData=cvCreateMat(numPts,2,CV_64FC1);  //Se crea una matrix que almacenara las posiciones de los pixeles
            posData=posFind(skImg,numPts);                 //Se hallan las posiciones de los pixeles del perfil
			numPts=cvCountNonZero(posData)/2;
            cout<<"\n\tPUNTOS DEL PERFIL\t"<<numPts;
            if(numPts<60) {
                minimalPts=true;
                return;
            }
            /****** Discretizacion de los datos *******/
            double tempV=0,nivel[60],paso=(numPts-1)/59.0;
            for(int i=0;i<60;i++){
				if(i==0) nivel[i]=0;
				else {
					nivel[i]=nivel[i-1]+paso;
					nivel[i-1]=tempV;
					tempV=round(nivel[i]);
				}
			}
			/******************************************/
            //cout<<"\n\tDISCRETIZACION";
            for(int i=0;i<60;i++){
				int temPos=int(nivel[i]);
				double discPt1=cvGetReal2D(posData,temPos,0);
				double discPt2=cvGetReal2D(posData,temPos,1);
				cvSetReal2D(newPosData,i,0,discPt1);
                cvSetReal2D(newPosData,i,1,discPt2);
			}
            cvCopy(newPosData,colorData,NULL);
            /******************************************/
			CvScalar refRow=cvScalar(cvGetReal2D(newPosData,0,0),0.0,0.0,0.0);
            CvScalar refCol=cvScalar(600.0,0.0,0.0,0.0);
            CvMat* dataRow=cvCreateMat(60,1,CV_64FC1);  //Se crea una matrix que almacena las FILAS
            CvMat* dataCol=cvCreateMat(60,1,CV_64FC1);  //Se crea una matrix que almacena las COLUMNAS
			cvGetCol(newPosData,dataRow,0);                    //Se transfiere la información de las filas a un vector
			cvGetCol(newPosData,dataCol,1);                    //Se transfiere la información de las columnas a un vector
			cvSubS(dataRow,refRow,dataRow,NULL);            //Se restan los valores de fila con una referencia
			cvSubS(dataCol,refCol,dataCol,NULL);            //Se restan los valores de fila con una referencia
			//DESTRUCTION
			cvReleaseImage( &grayImg );
			cvReleaseImage( &proImg );
            cvReleaseImage( &binImg );
            //cvReleaseImage( &skImg );
			//END OF DESTRUCTION
            CvMat* Rad_Height=cvCreateMat(60,2,CV_64FC1);
			Rad_Height=pix2mm(dataRow,dataCol);
			double pi=3.1415926535;
            //cout<<"\n\tTRANSFORMACION PIXEL A MM - iniciada";
            for(int i=0;i<60;i++){
				//int posNivel=int(nivel[i]);
				double x=cvGetReal2D(Rad_Height,i,0)*cos(0.01*number*pi);
				double y=cvGetReal2D(Rad_Height,i,0)*sin(0.01*number*pi);
				double z=cvGetReal2D(Rad_Height,i,1);
				cilindro[i].x=float(x);
				cilindro[i].y=float(y);
                cilindro[i].z=float(z);
            }
            //cout<<"\n\tTRANSFORMACION PIXEL A MM - finalizada";
        }
};


#endif // S3DFUNCTIONS_H_INCLUDED
