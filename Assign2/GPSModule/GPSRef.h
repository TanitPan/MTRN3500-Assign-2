#pragma once
#using <System.dll>

#define CRC32_POLYNOMIAL			0xEDB88320L


using namespace System;
using namespace System::Threading;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

ref class GPSRef
{
private:
	int PortNumber;
	System::String^ IPAddress;
	System::String^ ResponseData; // String to store received data for display

public:
	TcpClient^ Client;
	NetworkStream^ Stream;
	int ContinueFlag = 0;
	double Northing;
	double Easting;
	double Height;
	int ErrorCode = 0;
	unsigned int CalculatedCRC;
	unsigned int ServerCRC;
	array<unsigned char>^ RecvData; // Arrays of unsigned chars to send and receive data
public:
	GPSRef(System::String^ ipaddress, int port);
	~GPSRef();
	unsigned long CRC32Value(int i);
	unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char* ucBuffer);

	bool Connect(); // Setup and connect to TCP client
	void GetGPSData(); // Get GPS data information from server and store it in struct

	double GetNorthing();
	double GetEasting();
	double GetHeight();

};