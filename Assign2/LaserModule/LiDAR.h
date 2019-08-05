#pragma once

#include <SMObject.h>
#include <SMStructs.h>
#using <System.dll>

using namespace System;
using namespace System::Threading;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

ref class LiDAR
{
private:
	int PortNumber;
	System::String^ IPAddress;
	double StartAngle;
	double Resolution;
	System::String^ ResponseData; // String to store received data for display
	array<System::String^>^ Fragments; // Store splitted arrays

public:
	TcpClient^ Client;
	NetworkStream^ Stream;
	int NumRanges;
	int ContinueFlag = 0;
	array<double>^ Range;
	array<double>^ RangeX;
	array<double>^ RangeY;
	// arrays of unsigned chars to send and receive data
	array<unsigned char>^ SendData;
	array<unsigned char>^ ReadData;
public:
	LiDAR(System::String^ ipaddress, int port);
	bool Connect();
	void GetLaserScan();
	void GetXYRangeData();
	double GetStartAngle();
	double GetResolution();
};