//Compile in a C++ CLR empty project
//#include <SMObject.h>
//#include <SMStructs.h>
//#using <System.dll>
#include "LiDAR.h"

#include <conio.h>//_kbhit()
#define WAIT_TIME 40

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

using namespace System::Threading;

int main()
{

	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));

	PM* PMSMPtr = nullptr;
	int WaitCount;
	

	PMObj.SMAccess();
	if (PMObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed");
		return -2;
	}

	PMSMPtr = (PM*)PMObj.pData;
	PMSMPtr->Shutdown.Flags.Laser = 0;



	// LMS151 port number must be 2111
	int PortNumber = 23000;
	// Pointer to TcpClent type object on managed heap
	TcpClient^ Client;
	// arrays of unsigned chars to send and receive data
	array<unsigned char>^ SendData;
	array<unsigned char>^ ReadData;
	// String command to ask for Channel 1 analogue voltage from the PLC
	// These command are available on Galil RIO47122 command reference manual
	// available online
	System::String^ AskScan = gcnew System::String("sRN LMDscandata");

	System::String^ MyID = gcnew System::String("5224642\n");
	// String to store received data for display
	System::String^ ResponseData;
	// For data reading // 
	double StartAngle;
	double Resolution;
	double NumRanges;
	array<double>^ Range;
	array<double>^ RangeX;
	array<double>^ RangeY;



	// Creat TcpClient object and connect to it
	Client = gcnew TcpClient("192.168.1.200", PortNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;//ms The number is given by Tutor 2000
	Client->SendTimeout = 500;//ms  The number is given by Tutor 2000
	Client->ReceiveBufferSize = 2048; // used to be 1024
	Client->SendBufferSize = 1024;

	// unsigned char arrays of 16 bytes each are created on managed heap
	SendData = gcnew array<unsigned char>(16);
	ReadData = gcnew array<unsigned char>(2500);
	


	// Get the network streab object associated with clien so we 
	// can use it to read and write
	NetworkStream^ Stream = Client->GetStream();

	// Convert string command to an array of unsigned char 
	SendData = System::Text::Encoding::ASCII->GetBytes(MyID);
	// Store splitted arrays
	array<System::String^>^ Fragments = nullptr; 
	Stream->Write(SendData, 0, SendData->Length);
	// Read the incoming data
	Stream->Read(ReadData, 0, ReadData->Length);
	// Convert incoming data from an array of unsigned char bytes to an ASCII string
	ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	// Print the received string on the screen
	Console::WriteLine(ResponseData);
	
	// Convert string command to an array of unsigned char
	SendData = System::Text::Encoding::ASCII->GetBytes(AskScan);

	//Loop
	while (!PMSMPtr->Shutdown.Flags.Laser)
	{
		// Do some Heart beat stuff
		PMSMPtr->Heartbeats.Flags.Laser = 1;

		if (PMSMPtr->PMHeartbeats.Flags.Laser)
		{
			PMSMPtr->PMHeartbeats.Flags.Laser = 0;
			WaitCount = 0;
		}
		else
		{
			if (++WaitCount > WAIT_TIME)
			{
				PMSMPtr->Shutdown.Status = 0xFF;
			}
		}


		// Write command asking for data
		Stream->WriteByte(0x02);
		Stream->Write(SendData, 0, SendData->Length);
		Stream->WriteByte(0x03);
		// Wait for the server to prepare the data, 1 ms would be sufficient, but used 10 ms
		System::Threading::Thread::Sleep(10);
		// Read the incoming data
		Stream->Read(ReadData, 0, ReadData->Length);
		// Convert incoming data from an array of unsigned char bytes to an ASCII string
		ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);

		Fragments = ResponseData->Split(' ');
		//StartAngle = System::Convert

		// Print the received string on the screen
		Console::WriteLine(ResponseData);

		Sleep(20);
	}

	Stream->Close();
	Client->Close();

	Console::WriteLine("LaserMain terminated normally.");
	Console::ReadKey();
	//Console::ReadKey();


	return 0;
}