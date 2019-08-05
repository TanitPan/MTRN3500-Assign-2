#include "LiDAR.h"

LiDAR::LiDAR(System::String^ ipaddress, int port)
{
	IPAddress = ipaddress;
	PortNumber = port;
}

bool LiDAR::Connect()
{
	System::String^ MyID = gcnew System::String("5224642\n");

	// String command to ask for Channel 1 analogue voltage from the PLC
	// These command are available on Galil RIO47122 command reference manual
	// available online
	System::String^ AskScan = gcnew System::String("sRN LMDscandata");
	Fragments = nullptr;

	// Creat TcpClient object and connect to it
	Client = gcnew TcpClient(IPAddress, PortNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 2000;//ms The number is given by Tutor 2000
	Client->SendTimeout = 500;//ms  The number is given by Tutor 2000
	Client->ReceiveBufferSize = 2048; // used to be 1024
	Client->SendBufferSize = 1024;

	// unsigned char arrays of 16 bytes each are created on managed heap
	SendData = gcnew array<unsigned char>(16);
	ReadData = gcnew array<unsigned char>(2500);

	// Get the network stream object associated with client so we 
	// can use it to read and write
	Stream = Client->GetStream();

	// Convert string command to an array of unsigned char 
	SendData = System::Text::Encoding::ASCII->GetBytes(MyID);
	Stream->Write(SendData, 0, SendData->Length);
	// Read the incoming data
	Stream->Read(ReadData, 0, ReadData->Length);
	// Convert incoming data from an array of unsigned char bytes to an ASCII string
	ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);

	// Print the received string on the screen
	Console::WriteLine(ResponseData);

	// Convert string command to an array of unsigned char
	SendData = System::Text::Encoding::ASCII->GetBytes(AskScan);

	return true;
}

void LiDAR::GetLaserScan()
{
	// Write command asking for data
	Stream->WriteByte(0x02);  // SOT
	Stream->Write(SendData, 0, SendData->Length);
	Stream->WriteByte(0x03); // EOT
	// Wait for the server to prepare the data, 1 ms would be sufficient, but used 10 ms
	System::Threading::Thread::Sleep(10);
	// Read the incoming data
	Stream->Read(ReadData, 0, ReadData->Length);
	// Convert incoming data from an array of unsigned char bytes to an ASCII string
	ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	Fragments = ResponseData->Split(' ');
	//System::String^ ptr = Fragments[1];
	//if(System::String::Compare(Fragments[1],"LMDscandata"); dont need anymore?

	// Translating the ASCII data to usable values
	StartAngle = System::Convert::ToInt32(Fragments[23], 16);
	Resolution = (double)System::Convert::ToInt32(Fragments[24], 16) / 10000;
	NumRanges = System::Convert::ToInt32(Fragments[25], 16);

	Range = gcnew array<double>(NumRanges);
	RangeX = gcnew array<double>(NumRanges);
	RangeY = gcnew array<double>(NumRanges);

	if (NumRanges == 361)
	{
		for (int i = 0, a = 0; i < NumRanges; i++, a++)
		{
			if (a > 3) a = 0;
			Range[i] = System::Convert::ToInt32(Fragments[26 + i], 16);
			RangeX[i] = Range[i] * Math::Sin(i * Resolution * Math::PI / 180.0);
			RangeY[i] = -Range[i] * Math::Cos(i * Resolution * Math::PI / 180.0);
			

			//Console::WriteLine("{0,10:F3} {1,10:F3}", RangeX[a], RangeY[a]);
		}

		Console::WriteLine("StartAngle:{0,10:F3}  Resolution:{1,10:F3}  NumRanges:{2,10:D}", StartAngle, Resolution, NumRanges);
	
	}
	else
	{
		ContinueFlag = 1;
	}

}

void LiDAR::GetXYRangeData()
{
	Console::WriteLine("RangeX:{0,10:F3}      RangeY:{1,10:F3}", RangeX[0], RangeY[0]);
	Console::WriteLine("RangeX:{0,10:F3}      RangeY:{1,10:F3}", RangeX[1], RangeY[1]);
	Console::WriteLine("RangeX:{0,10:F3}      RangeY:{1,10:F3}\n", RangeX[2], RangeY[2]);
}

double LiDAR::GetStartAngle()
{
	return this->StartAngle;
}

double LiDAR::GetResolution()
{
	return this->Resolution;
}
