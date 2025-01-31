#pragma once

struct ModuleFlags
{
	unsigned char
		PM : 1,
		GPS : 1,
		Laser : 1,
		Xbox : 1,
		Vehicle : 1,
		DisplayGL : 1,
		Unused : 2;
};

union ExecFlags
{
	unsigned char Status;
	ModuleFlags Flags;
};

struct GPS
{
	double Easting;
	double Northing;
	double Height;
};

struct Laser
{
	double XRange[500];
	double YRange[500];
	int NumRanges;
};

struct PM
{
	ExecFlags Heartbeats;
	ExecFlags PMHeartbeats;
	ExecFlags Shutdown;
	double PMTimeStamp;

};

struct VehicleSM
{
	double actualSpeed;
	double actualSteering;
};

struct Remote
{
	double remoteSpeed;
	double remoteSteering;
	unsigned char routineShutdown;

};
