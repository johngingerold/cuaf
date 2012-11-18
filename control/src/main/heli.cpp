#include <stdlib.h>
#include <iostream>
#include "CHeli.h"
#include "CGui.h"
#include <cv.h>
#include <highgui.h>

using namespace cv;

CHeli *heli;
CRawImage *image;
CGui* gui;

int pidP = 0, pidI = 0, pidD = 0;

bool stop = false;
float pitch,roll,yaw,height;

// Process Keyboard Inputs.

void processKeys(char keyPress)
{
	// End Program.
	if (keyPress == 'q') stop = true;

	// Basic Commands.
	if (keyPress == 'z') heli->takeoff();
    if (keyPress == 'x') heli->land();
	if (keyPress == '[') heli->switchCamera(0);
	if (keyPress == ']') heli->switchCamera(1);

	if (keyPress == 'v') {
	    heli->setAltitude(1000);
        heli->altitudeControlLoop();
    } else
        heli->setAngles(0,0,0,0);

}

void onChangePid( int val, void* object)
{
    CPid* obj = (CPid*) object;
    obj->pGain = pidP / 1000.0;
    obj->iGain = pidI / 1000.0;
    obj->dGain = pidD / 1000.0;
    obj->reset();
}

int main(int argc,char* argv[])
{
	// Establishing connection with drone.
	heli = new CHeli();

	// Create GUI.
	gui = new CGui(320,240);

	//this class holds the image from the drone.
	image = new CRawImage(320,240);

	namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
	cv::createTrackbar( "PID P", "Display Image", &pidP, 1000, onChangePid, &(heli->altitudePid));
	cv::createTrackbar( "PID I", "Display Image", &pidI, 1000, onChangePid, &(heli->altitudePid));
	cv::createTrackbar( "PID D", "Display Image", &pidD, 1000, onChangePid, &(heli->altitudePid));


	while (stop == false){
		char keypress = waitKey(30);
		if(keypress != -1) {
			processKeys(keypress);
		}
		//fprintf(stdout,"Angles %.2lf %.2lf %.2lf ",helidata.phi,helidata.psi,helidata.theta);
		//fprintf(stdout,"Speeds %.2lf %.2lf %.2lf ",helidata.vx,helidata.vy,helidata.vz);
		//fprintf(stdout,"Battery %.0lf ",helidata.battery);

		// Stores the latest image from the drone in image.
		heli->renewImage(image);

		//imshow( "Display Image", image );

		// Get the commands from the keyboard, send them to the Ar.drone, then reset them.
		heli->setAngles(pitch,roll,yaw,height);
		pitch=roll=yaw=height=0.0;

		// Update GUI.
		gui->drawImage(image);
		gui->update();

		// Only refresh occasionally.
		usleep(20000);
	}

	// Cleanup.
	delete heli;
	delete image;
	delete gui;
	return 0;
}

