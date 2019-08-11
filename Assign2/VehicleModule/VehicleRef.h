#pragma once

#using <System.dll>

using namespace System;
using namespace System::Threading;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

ref class VehicleRef
{
private:
	int PortNumber;
	System::String^ IPAddress;
public:
	TcpClient^ Client;
	NetworkStream^ Stream;
	double speed;
	double steering;
	array<unsigned char>^ SendData; // Arrays of unsigned chars to send and receive data

public:
	~VehicleRef();
	VehicleRef(System::String^ ipaddress, int port);
	bool Connect(); // Setup client, connect and authenticate to server
	void ControlVehicle(double steering, double speed, unsigned int flag);
	double GetSpeed();
	double GetSteering();
};