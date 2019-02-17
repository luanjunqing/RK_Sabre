/*
 * @Author: LUAN Junqing
 * @Date: 2019-02-17 11:55:24
 * @Last Modified by: LUAN Junqing
 * @Last Modified time: 2019-02-17 17:45:55
 */


#include <stdio.h>
#include <stdlib.h>
#include "highgui.h"
#include "cv.h"

#define WINDOW_LEN (unsigned int)9

void imgToGray(IplImage *Image_IN, uchar *Data_out, int step_gray){
	int i, j;
	int step, channels, width, height;
	uchar *Data_in;
	step = Image_IN -> widthStep;
	channels  = Image_IN -> nChannels;
	width = Image_IN -> width;
	height = Image_IN -> height;
	Data_in = (uchar *)Image_IN -> imageData;
	for(i = 0; i < height; ++i){
		for(j = 0; j < width; ++j){
			Data_out[i*step_gray + j] = 0.114*Data_in[i*step + j*channels + 0] + 0.587*Data_in[i*step + j*channels + 1] + 0.299*Data_in[i*step + j*channels + 2];
		}
	}
}

int compare(const void *f1, const void *f2){
    return (*(char*)f1 > *(char*)f2) ? 1 : -1;
}

unsigned int sum(char tab[], unsigned int length){
    int i;
    unsigned int r=0;
    for(i=0; i < length; ++i){
        r += tab[i];
    }
    return r;
}

unsigned int sort(char window[], unsigned int length) {
    unsigned int r = 0;
    int tmp = 0;
	int i, j;
    for(i = 1; i < length; ++i){
        for(j = 0; j < length - i; ++j){
            if(window[j] > window[j+1]){
                tmp = window[j];
                window[j] = window[j+1];
                window[j+1] = tmp;
                r++;
            }
        }
    }
    qsort(window, length, sizeof(char), compare);
    return r;
}

void median_filter(IplImage *img_Filtered, IplImage *img_Original){
    int i, j, width, height, step_gray;
    uchar *Data_original, *Data_filtered;
    char window[WINDOW_LEN];
    width = MIN(img_Filtered -> width, img_Original -> width);
    height = MIN(img_Filtered -> height, img_Original -> height);
    Data_original = (uchar*)img_Original -> imageData;
    Data_filtered = (uchar*)img_Filtered -> imageData;
    step_gray = img_Filtered -> widthStep/sizeof(uchar);
    for(i = 1; i < height - 1; ++i){
        for(j = 1; j < width - 1; ++j){
            window[0] = Data_original[i*step_gray + j];
            window[1] = Data_original[i*step_gray + (j-1)];
            window[2] = Data_original[i*step_gray + (j+1)];

            window[3] = Data_original[(i+1)*step_gray + (j)];
            window[4] = Data_original[(i+1)*step_gray + (j-1)];
            window[5] = Data_original[(i+1)*step_gray + (j+1)];

            window[6] = Data_original[(i-1)*step_gray + (j)];
            window[7] = Data_original[(i-1)*step_gray + (j-1)];
            window[8] = Data_original[(i-1)*step_gray + (j+1)];
            sort(window, WINDOW_LEN);
            Data_filtered[i*step_gray + j] = window[4];
        }
    }
}

void filter_sobelX(IplImage *img_Filtered, IplImage *img_Original){
    int i, j, width, height, step_gray;
    uchar *Data_original, *Data_filtered;
    char window[WINDOW_LEN];
    char tmp_img;
    width = MIN(img_Filtered -> width, img_Original -> width);
    height = MIN(img_Filtered -> height, img_Original -> height);
    Data_original = (uchar*)img_Original -> imageData;
    Data_filtered = (uchar*)img_Filtered -> imageData;
    step_gray = img_Filtered -> widthStep/sizeof(uchar);
    for(i = 1; i < height - 1; ++i){
        for(j = 1; j < width - 1; ++j){
            window[0] = (char)Data_original[i * step_gray + j] * 0;
            window[1] = (char)Data_original[i * step_gray + (j - 1)] * -1;
            window[2] = (char)Data_original[i * step_gray + (j + 1)];

            window[3] = (char)Data_original[(i + 1) * step_gray + (j)] * 0;
            window[4] = (char)Data_original[(i + 1) * step_gray + (j - 1)] * -2;
            window[5] = (char)Data_original[(i + 1) * step_gray + (j + 1)] * 2;

            window[6] = (char)Data_original[(i - 1) * step_gray + j] * 0;
            window[7] = (char)Data_original[(i - 1) * step_gray + (j - 1)] * -1;
            window[8] = (char)Data_original[(i - 1) * step_gray + (j + 1)];
            tmp_img = sum(window, WINDOW_LEN);
            if(tmp_img < 0){
                Data_filtered[i * step_gray + j] = 0;
            }
			else{
                Data_filtered[i * step_gray + j] = tmp_img;
            }
        }
    }
}

void filter_sobelY(IplImage *img_Filtered, IplImage *img_Original){
    int i, j, width, height, step_gray;
    uchar *Data_original, *Data_filtered;
    char window[WINDOW_LEN];
    char tmp_img;
    width=MIN(img_Filtered->width, img_Original->width);
    height=MIN(img_Filtered->height, img_Original->height);
    Data_original=(uchar *)img_Original->imageData;
    Data_filtered=(uchar *)img_Filtered->imageData;
    step_gray=img_Filtered->widthStep/sizeof(uchar);
    for(i=1;i<height-1;++i){
        for(j=1;j<width-1;++j){
            window[0] = (char)Data_original[i * step_gray + j] * -2;
            window[1] = (char)Data_original[i * step_gray + (j-1)] * -1;
            window[2] = (char)Data_original[i * step_gray + (j+1)] * -1;
            window[3] = (char)Data_original[(i+1) * step_gray + j] * 0;
            window[4] = (char)Data_original[(i+1) * step_gray + (j-1)] * 0;
            window[5] = (char)Data_original[(i+1) * step_gray + (j+1)] * 0;
            window[6] = (char)Data_original[(i-1) * step_gray + j] * 2;
            window[7] = (char)Data_original[(i-1) * step_gray + (j-1)];
            window[8] = (char)Data_original[(i-1) * step_gray + (j+1)];
            tmp_img = sum(window, WINDOW_LEN);
            if(tmp_img < 0){
                Data_filtered[i * step_gray + j] = 0;
            }
            else{
                Data_filtered[i * step_gray + j] = tmp_img;
            }
        }
    }
}



int main(int argc, char const *argv[]){
    // Touche clavier pour quitter
    char ESC_keyboard;
	// intialisation de la variable ESC_keyboard
	ESC_keyboard = cvWaitKey(5);
    // Images
	IplImage *Image_IN;
	IplImage *Image_OUT;
	IplImage *Image_MED;
	IplImage *Image_SOX;
	IplImage *Image_SOY;

	// Capture vidéo
    CvCapture *capture;

	// variables images
	int height,width,step,channels;
	uchar *Data_in;
	uchar *Data_out;
	uchar *Data_Med;
	uchar *Data_Sox;
	uchar *Data_Soy;


	// int i,j,k;
	int i, j;
    // Ouvrir le flux vidéo
    capture = cvCreateCameraCapture(CV_CAP_ANY);
	// capture = cvCreateCameraCapture(0);
    // capture = cvCreateCameraCapture(4);

    // Vérifier si l'ouverture du flux est ok
    if (!capture) {
		printf("Ouverture du flux vidéo impossible !\n");
		return 1;
    }

    // Définition des fenêtres
	cvNamedWindow("Image_IN_Window", CV_WINDOW_AUTOSIZE);   // Image_IN
	cvNamedWindow("Image_OUT_Window", CV_WINDOW_AUTOSIZE); 	// Image_OUT
	cvNamedWindow("Image_MED_Window", CV_WINDOW_AUTOSIZE); 	// Image_MED
	cvNamedWindow("Image_SOX_Window", CV_WINDOW_AUTOSIZE); 	// Image_SOX
	cvNamedWindow("Image_SOY_Window", CV_WINDOW_AUTOSIZE); 	// Image_SOY

	// Positionnement des fenêtres
    cvMoveWindow("Image_IN_Window", 0, 0);
    cvMoveWindow("Image_OUT_Window", 0, 500);
	cvMoveWindow("Image_MED_Window", 500, 0);
	cvMoveWindow("Image_SOX_Window", 500, 500);
	cvMoveWindow("Image_SOY_Window", 1000, 500);
	// cvMoveWindow("Image_SOM_Window", 1400,0);

	// Première acquisition
	Image_IN = cvQueryFrame(capture);
	// Création des images de sortie
	Image_OUT = cvCreateImage(cvSize(Image_IN -> width, Image_IN -> height),  IPL_DEPTH_8U, 1);
	Image_MED = cvCreateImage(cvSize(Image_IN -> width, Image_IN -> height),  IPL_DEPTH_8U, 1);
	Image_SOX = cvCreateImage(cvSize(Image_IN->width,Image_IN->height),  IPL_DEPTH_8U, 1);
	Image_SOY = cvCreateImage(cvSize(Image_IN->width,Image_IN->height),  IPL_DEPTH_8U, 1);

	int step_gray = Image_OUT -> widthStep/sizeof(uchar);


    while(ESC_keyboard != 'q' && ESC_keyboard != 'Q') {
		// On récupère une Image_IN
		Image_IN = cvQueryFrame(capture);
		// Dimension
		height    = Image_IN -> height;
		width     = Image_IN -> width;
		// distance entre les deux premiers pixels de lignes successives
		step      = Image_IN -> widthStep;
		channels  = Image_IN -> nChannels;
		// initialisation des pointeurs de donnée
		Data_in  = (uchar*) Image_IN -> imageData;
		Data_out = (uchar*) Image_OUT -> imageData;
		Data_Med = (uchar*) Image_MED -> imageData;
		Data_Sox = (uchar*) Image_SOX -> imageData;
		Data_Soy = (uchar*) Image_SOY ->imageData;

		imgToGray(Image_IN, Data_out, step_gray);
	    median_filter(Image_MED, Image_OUT);
		filter_sobelX(Image_SOX, Image_MED);
		filter_sobelY(Image_SOY, Image_MED);

		cvShowImage("Image_IN_Window", Image_IN);
		cvShowImage("Image_OUT_Window", Image_OUT);
		cvShowImage("Image_MED_Window", Image_MED);
		cvShowImage( "Image_SOX_Window", Image_SOX);
		cvShowImage( "Image_SOY_Window", Image_SOY);

		// On attend 5ms
		ESC_keyboard = cvWaitKey(5);
    }

    // Fermeture de l'acquisition Vidéo
	cvReleaseCapture(&capture);

	// Fermeture des fenêtres d'affichage
	cvDestroyWindow("Image_IN_Window");
    cvDestroyWindow("Image_OUT_Window");
	cvDestroyWindow("Image_MED_Window");
	cvDestroyWindow("Image_SOX_Window");
	cvDestroyWindow("Image_SOY_Window");

	return 0;
}
