// hello.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>

#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

#ifdef _EiC
#define WIN32
#endif

static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;



//declare struct
struct Video{
	IplImage* frame;
	CvCapture* capture;
	const char* inputFile;
	const char* windowName;
	int fps;
	int numFrame;
	int windowWidth;
	int windowHigh;
	//int posFrame;
	//float posMsec;
	//float ratio;
};


//declare global variable


//declare function
int queryVideoFrame(Video& file);
int loadVideoFile(Video& file, const char* filePath);
void closeWindow(const char* name,int type);
void newWindow(const char* name, int width, int high);
int signDetection( IplImage*& image, Video& file );
void detect_and_draw( IplImage*& img );