#include "input.h"

const char* cascade_name = "cascade/nopark_F_cascade.xml";
/*    "haarcascade_profileface.xml";*/

//Video input;

int main( int argc, char** argv ) {
	Video input;
	input.windowName = "Original Video";
	char* input_name = "input/test_1.avi";
	int i,j;
	if(loadVideoFile(input, input_name)==0){
		printf("width %d, high %d\n",input.windowWidth, input.windowWidth);
		printf("Frame rate = %d\nFrame count = %d\n",input.fps, input.numFrame);
		newWindow(input.windowName,input.windowWidth,input.windowHigh);
		queryVideoFrame(input);
	}
	else
	{
		printf("fail to load video file.\n");
	}
	
	cvWaitKey(0);
	cvReleaseCapture( &input.capture );
	return 0;
}

int queryVideoFrame(Video& file){
	if(!file.capture)
		return -1;
	file.frame = cvQueryFrame(file.capture );
	int i;
	for(i=1;i<=file.numFrame;i++){
		//file.posFrame = cvGetCaptureProperty( file.capture, CV_CAP_PROP_POS_FRAMES );
		//file.posMsec = cvGetCaptureProperty( file.capture, CV_CAP_PROP_POS_MSEC );
		//file.ratio = cvGetCaptureProperty( file.capture, CV_CAP_PROP_POS_AVI_RATIO );
		cvSetCaptureProperty( file.capture, CV_CAP_PROP_POS_AVI_RATIO,(double)i/file.numFrame);
		file.frame = cvQueryFrame( file.capture );
		signDetection(file.frame, file);
		cvShowImage(file.windowName, file.frame);
		cvWaitKey(file.fps);
		
		//loop video
		if(i==file.numFrame){
			i=0;
		}
	}
	return 0;
}

int loadVideoFile(Video& file, const char* filePath){
	file.inputFile = filePath;
	file.capture = cvCreateFileCapture(file.inputFile);
	if(!file.capture)
		return -1;
	cvGrabFrame(file.capture);
	file.windowHigh = cvGetCaptureProperty( file.capture, CV_CAP_PROP_FRAME_HEIGHT );
	file.windowWidth = cvGetCaptureProperty( file.capture, CV_CAP_PROP_FRAME_WIDTH );
	file.fps = cvGetCaptureProperty( file.capture, CV_CAP_PROP_FPS );
	file.numFrame = cvGetCaptureProperty( file.capture, CV_CAP_PROP_FRAME_COUNT );
	return 0;
}

int signDetection( IplImage*& image, Video& file )
{
    //CvCapture* capture = 0;
    //IplImage *frame, *frame_copy = 0;
    //int optlen = strlen("--cascade=");
    //const char* input_name = "test.avi";

    /*if( argc > 1 && strncmp( argv[1], "--cascade=", optlen ) == 0 )
    {
        cascade_name = argv[1] + optlen;
        input_name = argc > 2 ? argv[2] : 0;
    }
    else
    {
        cascade_name = "cascade/nopark_F_cascade.xml";
        input_name = argc > 1 ? argv[1] : 0;
    }
*/
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
    
    if( !cascade )
    {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
       // fprintf( stderr,
       // "Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
        return -1;
    }
    storage = cvCreateMemStorage(0);
    
    //if( !input_name || (isdigit(input_name[0]) && input_name[1] == '\0') )
    //    capture = cvCaptureFromCAM( !input_name ? 0 : input_name[0] - '0' );
    //else
        //capture = cvCaptureFromAVI( input_name ); 

    //cvNamedWindow( "DETECTION", 1 );
	//printf("capture= %s\n",capture);
    /*if( capture )
    {
        for(;;)
        {
            if( !cvGrabFrame( capture ))
                break;
            frame = cvRetrieveFrame( capture );
            if( !frame )
                break;
            if( !frame_copy )
                frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );
            if( frame->origin == IPL_ORIGIN_TL )
                cvCopy( frame, frame_copy, 0 );
            else
                cvFlip( frame, frame_copy, 0 );
            
            detect_and_draw( frame_copy );

            if( cvWaitKey( 10 ) >= 0 )
                break;
        }

        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &capture );
    }
    else
    {*/
        //const char* filename = input_name ? input_name : (char*)"lena.jpg";
        //IplImage* image = cvLoadImage( filename, 1 );

        if( image )
        {
            detect_and_draw( image );
            //cvWaitKey(0);
            //cvWaitKey(file.fps);
			//cvReleaseImage( &image );
        }
        else
        {
            /* assume it is a text file containing the
               list of the image filenames to be processed - one per line */
            //FILE* f = fopen( filename, "rt" );
			FILE* f = fopen( "default.jpg", "rt" );
            if( f )
            {
                char buf[1000+1];
                while( fgets( buf, 1000, f ) )
                {
                    int len = (int)strlen(buf);
                        len--;
                    buf[len] = '\0';
                    image = cvLoadImage( buf, 1 );
                    while( len > 0 && isspace(buf[len-1]) )
                    if( image )
                    {
                        detect_and_draw( image );
                        cvWaitKey(0);
                        cvReleaseImage( &image );
                    }
                }
                fclose(f);
            }
        }

   // }
    
    //cvDestroyWindow("DETECTION");

    return 0;
}

void detect_and_draw( IplImage*& img )
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };

    double scale = 1.3;
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)),
                     8, 1 );
    int i;

    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( storage );

    if( cascade )
    {
        double t = (double)cvGetTickCount();
        CvSeq* sign = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 4, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            cvSize(30, 30) );
		
        t = (double)cvGetTickCount() - t;
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
        for( i = 0; i < (sign ? sign->total : 0); i++ )
		{
            CvRect* r = (CvRect*)cvGetSeqElem( sign, i );
            CvPoint point1, point2;
            int radius;
            point1.x = cvRound((r->x)*scale);
            point1.y = cvRound((r->y)*scale);
            point2.x = cvRound((r->x + r->width)*scale);
            point2.y = cvRound((r->y + r->height)*scale);
			cvRectangle( img, point1, point2, {0,0,255}, 1, 8, 0 );
        }
    }

    //cvShowImage( "DETECTION", img );colors[i]
    //cvReleaseImage( &gray );
    //cvReleaseImage( &small_img );

}


void newWindow(const char* name, int width, int high){
	if(width==0 && high==0)
		cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
	else{
		cvNamedWindow(name,0);
		cvResizeWindow(name, width, high );
	}
}

void closeWindow(const char* name,int type) // 0 is to close single window, 1 is to close all windows
{
	if(type==0)
		cvDestroyWindow(name);
	else{
		cvDestroyAllWindows();
	}
}

