#pragma config(Sensor, dgtl1,  red,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl2,  green,          sensorLEDtoVCC)
#pragma config(Sensor, dgtl3,  stationLimit,   sensorTouch)
#pragma config(Sensor, dgtl4,  control,   sensorTouch)
#pragma config(Motor,  port2,  motorTop,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,  motorBottom,    tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

volatile bool isReady = false;
volatile bool isLoading = false;

int globalSpeed = 64;
int globalWait = 5000; /* Time in msec */

task checkReady () {
	while (1) {
		if (isReady) {
			turnLedOn(green);
			turnLedOff(red);
		}
		else {
			turnLedOn(red);
			turnLedOff(green);
		}
	}
}

task setEmpty () {
    while (1) {
        if (SensorValue(stationLimit)) {
            isReady = false;
        }
        else {
            if (isLoading) {
                isReady = false;
            }
            else {
                isReady = true;
            }
        }
    }
}

void load ()
{
	if (isLoading || !isReady) {
		return;
	}
	isLoading = true;
	motor[motorTop] = globalSpeed;
	while (isEmpty());
	motor[motorTop] = 0;
	
	sleep (globalWait);
	
	motor[motorBottom] = globalSpeed;
	while (!isEmpty());
	motor[motorBottom] = 0;
	isLoading = false;
}

task main()
{
	startTask (checkReady);
	startTask (setEmpty);
	while (1) {
	    if (SensorValue(control)) {
	        load();
	    }
	}
}