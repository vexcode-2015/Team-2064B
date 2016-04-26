#pragma config(Sensor, dgtl11, wheelEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           Intake,        tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           TRL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           TL,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           BRL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           BL,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           TR,            tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           TLL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           BR,            tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           BLL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          Convey,        tmotorVex393_HBridge, openLoop, reversed)

int target=0;	//define target and rpm at global level so it can be passed between tasks
//int motorSpeed=0;
float rpm;
int lesser;
int higer;
int normal;
int currentEncoder;	//Define variables to use
int previousEncoder;
int deltaEncoder;
int currentTime;
int previousTime;
int deltaTime;
task speedCalculate();	//define task(s) to be used

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(105)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

void pre_auton()
{
	bStopTasksBetweenModes = true;
}
task autonomous()
{
	while(true){
		motor[TLL] = -91;
		motor[TRL] = 91;
		motor[BLL] = 91;
		motor[BRL] = -91;

		wait1Msec (3000);
		motor[TLL] = -91;
		motor[TRL] = 91;
		motor[BLL] = 91;
		motor[BRL] = -91;
		motor[Intake] = -125;
		motor[Convey] = 125;
		wait1Msec(12000);

	}

}

task usercontrol()
{
	//	SensorValue[greenLED]=0;
	//	startTask(speedControl);
	startTask(speedCalculate);

	while (true)
	{
//START INTAKE
		if (vexRT[Btn5UXmtr2] == 1){// Intake All On
			motor[Intake] = -125;
			motor[Convey] = 125;}

		if (vexRT[Btn7RXmtr2] == 1){// Intake All On
			motor[Intake] = 125;
			motor[Convey] = -125;}

		if (vexRT[Btn5DXmtr2] == 1){// Intake Stop
			motor[Intake] = 0;
			motor[Convey] = 0;}

		if (vexRT[Btn7UXmtr2] == 1){// Front Intake On
			motor[Intake] = -125;}

		if (vexRT[Btn7DXmtr2] == 1){// Back Intake On
			motor[Convey] = 125;}

		if (vexRT[Btn7LXmtr2] == 1){// Back Intake Off
			motor[Convey] = 0;}

		if (vexRT[Btn7RXmtr2] == 1){// Front Intake Off
			motor[Intake] = 0;}

//START FLYWHEEL, speeds in no particular order
		if (vexRT[Btn8RXmtr2] == 1){// midcourt
			target = 395;
			lesser = 50;
			higer = 125;
			normal = 75;
			motor[TLL] = -75;
			motor[TRL] = 75;
			motor[BLL] = 75;
			motor[BRL] = -75;
		}
		else	if (vexRT[Btn8DXmtr2] == 1){// BAR
			target = 320;
			lesser = 45;
			higer = 125;
			normal = 70;
			motor[TLL] = -70;
			motor[TRL] = 70;
			motor[BLL] = 70;
			motor[BRL] = -70;}
		else	if (vexRT[Btn8LXmtr2] == 1){// Midcourt 2
			target = 360;
			lesser = 50;
			higer = 125;
			normal = 73;
			motor[TLL] = -73;
			motor[TRL] = 73;
			motor[BLL] = 73;
			motor[BRL] = -73;}
		else if (vexRT[Btn8UXmtr2] == 1){// Launcher Preset Speed TILE(FULL BATTERY) - no bang bang, didn't have enough time to tune accuracy
			target = 450;
			lesser = 90;
			higer = 90;
			normal = 90;
			motor[TLL] = -90;
			motor[TRL] = 90;
			motor[BLL] = 90;
			motor[BRL] = -90;}
		else if (vexRT[Btn6UXmtr2] == 1){// Launcher Preset Speed TILE(Lower BATTERY)
			target = 450;
			lesser = 70;
			higer = 125;
			normal = 95;
			motor[TLL] = -95;
			motor[TRL] = 95;
			motor[BLL] = 95;
			motor[BRL] = -95;}
		else if (vexRT[Btn8R] == 1){// Launcher Preset Speed TILE (Lower BATTERY)- no bang bang, didn't have enough time to tune accuracy
			target = 450;
			lesser = 85;
			higer = 85;
			normal = 85;
			motor[TLL] = -85;
			motor[TRL] = 85;
			motor[BLL] = 85;
			motor[BRL] = -85;}
		else if (vexRT[Btn8L] == 1){// Launcher Preset Speed TILE(Lower BATTERY)- no bang bang, didn't have enough time to tune accuracy
			target = 450;
			lesser = 87;
			higer = 87;
			normal = 87;
			motor[TLL] = -87;
			motor[TRL] = 87;
			motor[BLL] = 87;
			motor[BRL] = -87;}
//Actual BANG BANG code, simplest form
		else	if (rpm > target) {
			motor[TLL] = -lesser;
			motor[TRL] = lesser;
			motor[BLL] = lesser;
			motor[BRL] = -lesser;
			}else if (rpm == target) {
			motor[TLL] = -normal;
			motor[TRL] = normal;
			motor[BLL] = normal;
			motor[BRL] = -normal;
			}else	if (rpm < target) {
			motor[TLL] = -higer;
			motor[TRL] = higer;
			motor[BLL] = higer;
			motor[BRL] = -higer;
		}
//START BASE PROGRAM - tank drive
		motor[TR] = vexRT[Ch3] - vexRT[Ch1] - vexRT[Ch4];
		motor[BR] = vexRT[Ch3] - vexRT[Ch1] + vexRT[Ch4];
		motor[TL] = vexRT[Ch3] + vexRT[Ch1] + vexRT[Ch4];
		motor[BL] = vexRT[Ch3] + vexRT[Ch1] - vexRT[Ch4];
	}

}

task speedCalculate() //self explanatory, same speed calculate can be used in other velocity control loops - for future reference
{

	while(true)
	{
		previousEncoder = SensorValue[wheelEncoder]; //Measure Encoder and time at beginning
		previousTime = nSysTime;
		wait1Msec(25); //Wait to measure changes
		currentEncoder = SensorValue[wheelEncoder]; //Measure Encoder and time after delay
		currentTime = nSysTime;
		deltaEncoder = currentEncoder - previousEncoder; //Calculate changes
		deltaTime = currentTime - previousTime;
		rpm = (deltaEncoder*60)*(1000/deltaTime)/360; //calculate rpm but converrt degress to rotations, ms to s, s to minutes
		wait1Msec(10); //Wait to allow calculations, as well as not hog CPU
	}
}
