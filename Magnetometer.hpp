/*=====================================================================================
	Filename:	Magnetometer.hpp
	Version :	Draft A
	Date	: 	29-October-2021
	Author	:	Roger Thompson
=====================================================================================*/


/*****************************************************************************************
DESCRIPTION:
	This sketch reads data from the MHC5883L triple-axis magnetometer on the G-271
	breakout board.  Itis based on an example sketch written by Kevin Townsend for
	Adafruit Industries


 GY-271 pin connections:
    Vcc         5V rail (the GY-271 has an onboard 3.3V regulator to supply the MHC5883
    Ground      Ground rail
    SCL (I2C)   GPIO22 on the ESP32 (SCL on the Uno or A5 on the Nano)
    SDA (I2C)   GPIO21 on the ESP32 (SDA on the Uno  or A4 on the Nano)
    DRDY        N/C  (This is the Data Ready output, used to maximise sampling rate)

  The I2C device address is 0x1E  (this value is set in the Adafruit_HMC5883_U.h file)

  The nominal bandwidth for the HMC5883L is 15 samples per second.

*******************************************************************************************/

#include <Adafruit_HMC5883_U.h>
#include <Adafruit_Sensor.h>
#ifndef WIRE_H
	#include <Wire.h>
	#define WIRE_H
#endif


#define ROLLING_AVERAGE_LENGTH 20

class RollingAverage {  // declaration/definition of the RollingAverage class
public:
	// the default constructor
	RollingAverage() {
		index = 0;
		rollingTotal = 0;
	}

	//method to update rolling average with the latest reading
	float updateRav(float newValue) {
		rollingTotal -= buffer[index];  // subtract oldest value from rolling total
		rollingTotal += newValue;		// add the newValue to the rolling total
		buffer[index] = newValue;       // insert new value in the buffer
		index = (index + 1) % ROLLING_AVERAGE_LENGTH;
		return rollingTotal/ROLLING_AVERAGE_LENGTH;
	}
private:
	// member variables
	float buffer[ROLLING_AVERAGE_LENGTH];  // cyclic buffer to hold the last N readings
	int index; 			// the index for the cyclic buffer, labelling the entry point for the next reading
	float rollingTotal; // the sum of the last N readings
};



/* Create an instance of the Adafruit_HMC5883_Unified object, and assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(10297);




void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


/*void setup(void)
{
  Serial.begin(115200);
  Serial.println("HMC5883 Magnetometer Test (29/10/2021)"); Serial.println("");

   Initialise the sensor
  if(!mag.begin())
  {
     There was a problem detecting the HMC5883 ... check your connections
    Serial.println("ERROR CODE 200: No HMC5883 detected ... Check your wiring!");
    while(1);
  }

   Display some basic information on this sensor
  displaySensorDetails();
}*/
// The following calibration data is based on measureme
// This was the third calibration, after the offsets had
#define X_OFFSET (+ 39.1)
#define Y_OFFSET (+ 32.75)
#define Y_SENSITIVITY_CORRECTION -0.9365

RollingAverage xRollingAv;
RollingAverage yRollingAv;
RollingAverage headingRollingAv;

/*
void loop(void) {
	float xVal, yVal;  // raw x- and y- values from the magnetometer
	float xRav, yRav, hRav;
	 Get a new sensor event
	sensors_event_t event;
	mag.getEvent(&event);

	xVal = event.magnetic.x + X_OFFSET;
	yVal = (event.magnetic.y + Y_OFFSET) * Y_SENSITIVITY_CORRECTION;
	xRav = xRollingAv.updateRav(xVal);
	yRav = yRollingAv.updateRav(yVal);

	 Display the results (magnetic vector values are in micro-Tesla (uT))
	Serial.print("X: "); Serial.print(xVal); Serial.print("  "); Serial.print(xRav); Serial.print(" \t  ");
	Serial.print("Y: "); Serial.print(yVal); Serial.print("  ");Serial.print(yRav); Serial.print("  \t ");
	Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.print("uT");

	// Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
	// Calculate heading when the magnetometer is level, then correct for signs of axis.
	float heading = atan2(yVal, xVal);
	//float heading = atan2(yRav, xRav);


	//float declinationAngle = 0.0;  // Magnetic variation (declination) s near zero in Felixstowe
	//heading += declinationAngle;  // correction for magnetic variation

	// Correct for when signs are reversed.
	if(heading < 0)
		heading += 2*PI;

	// Check for wrap due to addition of declination.
	if(heading > 2*PI)
		heading -= 2*PI;

	// Convert radians to degrees for readability.
	float headingDegrees = heading * 180/M_PI;
	hRav = headingRollingAv.updateRav(headingDegrees);

	Serial.print("\t  Heading (degrees): "); Serial.print(headingDegrees);Serial.print("  "); Serial.println(hRav);

	delay(200);
}

*/


