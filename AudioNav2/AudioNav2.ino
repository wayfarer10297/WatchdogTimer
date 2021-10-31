/*====================================================================================
Filename:	AudioNav2.ino
Version	:	Draft A
Date	: 	29-October-2021
Author	:	Roger Thompson
=====================================================================================*/


/**************************************************************************************
DESCRIPTION

	This is the master sketch for the prototype AudioNav unit.

	It makes use of the Scheduler Class written by Bob Whitehouse to cue up (at the
	appropriate intervals) the tasks needed to drive the hardware attached to the
	ESP32.

	The code associated with the various ESP32 peripherals is, where possible,
	encapsulated within Classes and/or Functions, the code for which is contained
	within separate dedicated .hpp files. These .hpp files are stored in the AudiNav2
	Project folder in Sloeber(Eclipse).

	The following 13 libraries are used by AudioNav:

		Adafruit_GPS_Library
		Adafruit_HMC5883_Unified
		Adafruit_Unified_Sensor
		ESP8266Audio
		FS
		HTTPClient  (check where/if this is needed?!)
		SD
		SPI
		SPIFFS
		WiFi
		WiFiClientSecure
		Wire
		LinkedList
**************************************************************************************/

#ifndef SCHEDULER_HCC
	#include "Scheduler.hpp"
	#define SCHEDULER_HCC
#endif



#ifndef MAGNETOMETER_HCC
	#include "Magnetometer.hpp"
	#define MAGNETOMETER_HCC
#endif


#include <esp_task_wdt.h>  // Required to disable the Watchdog timer (pro tem)


Scheduler scheduler;	// Create an instance of the Task Scheduler

// Create the tasks to be scheduled
Task getGpsTask;		// GPS course and speed.  The task is defined in GPS.hpp
Task getHeadingTask;  	// Magnetic heading.  The task is defined in Magnetometer.hpp
Task getRotaryEncoderTask;  // Rotary Encoder input and associated Menu actions
Task serviceAnnouncementsTask;  // Audio announcement handler
Task SDcardTask;  		// SD card reader handler


// dummy task function
void getGPSdata() {
  Serial.print("GPS COG = 195    ");
  Serial.println("GPS SOG = 6.3  ");
}

// dummy task function
void getHeading() {
  Serial.println("Heading = 360");
}



void setup() {
	Serial.begin(115200);

	// Set up task to read the GPS 	receiver every 1000ms
	getGpsTask.proc = &getGPSdata;
	getGpsTask.repeating = true;
	getGpsTask.period = 200000;      // -- execute every 200ms
	scheduler.add(&getGpsTask);

	// Set up task to read the Magnetometer every 200ms
	getHeadingTask.proc = &getHeading;
	getHeadingTask.repeating = true;
	getHeadingTask.period =100000;      // -- execute every 100ms
	scheduler.add(&getHeadingTask);

	esp_task_wdt_init(30, false);  // Disable the watchdog timer (pending further discussion with Bob)
}



void loop() {
	// Run the scheduler
    scheduler.dispatch();
}
