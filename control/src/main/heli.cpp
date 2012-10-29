#include <stdlib.h>
#include <iostream>
#include "CHeli.h"
#include "CGui.h"

CHeli *heli;
CRawImage *image;
Uint8 *keys = NULL;
CGui* gui;

bool stop = false;
float pitch,roll,yaw,height;

// Process Keyboard Inputs.
void processKeys()
{
	keys = SDL_GetKeyState(NULL);
	
	// End Program.
	if (keys[SDLK_ESCAPE]) stop = true;

	// Sets variables to move drone.
	if (keys[SDLK_KP7])  yaw = -20000.0;
	if (keys[SDLK_KP9])  yaw = 20000.0;
	if (keys[SDLK_KP4])  roll = -20000.0;
	if (keys[SDLK_KP6])  roll = 20000.0;
	if (keys[SDLK_KP8])  pitch = -20000.0;
	if (keys[SDLK_KP2])  pitch = 20000.0;
	if (keys[SDLK_KP_PLUS])  height = 20000.0;
	if (keys[SDLK_KP_MINUS])  height = -20000.0;

	// Change Camera.
	if (keys[SDLK_z]) heli->switchCamera(0);
	if (keys[SDLK_x]) heli->switchCamera(1);

	// Basic Commands.
	if (keys[SDLK_q]) heli->takeoff();
	if (keys[SDLK_a]) heli->land();
}

int main(int argc,char* argv[])
{
	// Establishing connection with drone.
	heli = new CHeli();

	// Create GUI.
	gui = new CGui(320,240);

	//this class holds the image from the drone.
	image = new CRawImage(320,240);
	
	while (stop == false){		
		fprintf(stdout,"Angles %.2lf %.2lf %.2lf ",helidata.phi,helidata.psi,helidata.theta);
		fprintf(stdout,"Speeds %.2lf %.2lf %.2lf ",helidata.vx,helidata.vy,helidata.vz);
		fprintf(stdout,"Battery %.0lf ",helidata.battery);

		// Stores the latest image from the drone in image.
		heli->renewImage(image);

		// Get the commands from the keyboard, send them to the Ar.drone, then reset them.
		processKeys();		
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

