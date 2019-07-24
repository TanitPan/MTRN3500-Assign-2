#pragma once

#include <SMObject.h>
#include <SMStructs.h>

ref class LiDAR
{
private:
	TcpClient^ Client;
	int PortNumber;
	String^ IPAddress;
	double StartAngle;
	double Resolution;
	String^ Message;

public:
	int NumRanges;
	array<double>^ Ranges;
	array<double>^ RangeX;
	array<double>^ RangeY;
public:
	LiDAR(String^ ipaddress, int port);
	bool Connect();
	void GetXYRangeData();
};