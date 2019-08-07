#pragma once

struct ModuleFlags
{
	unsigned char
		PM : 1,
		GPS : 1,
		Laser : 1,
		Xbox : 1,
		Vehicle : 1,
		Unused : 3;
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
	double XRange[600];
	double YRange[600];
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

};

struct Remote
{

};