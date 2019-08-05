#include "GPSRef.h"
#pragma pack(1)

// Novatel GPS BESTUTMB data structure. 112 bytes
struct GPSData
{
	unsigned int Header;
	unsigned char Discards1[40];
	double Northing;
	double Easting;
	double Height;
	unsigned char Discards2[40];
	unsigned int Checksum;
} NovatelGPS;

GPSRef::GPSRef(System::String^ ipaddress, int port)
{
	IPAddress = ipaddress;
	PortNumber = port;
}

GPSRef::~GPSRef()
{
	Stream->Close();
	Client->Close();
}

unsigned long GPSRef::CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for (j = 8; j > 0; j--)
	{
		if (ulCRC & 1)
			ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
	}
	return ulCRC;
}

/* --------------------------------------------------------------------------
Calculates the CRC-32 of a block of data all at once
-------------------------------------------------------------------------- */
unsigned long GPSRef::CalculateBlockCRC32(unsigned long ulCount, unsigned char* ucBuffer)
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xff);
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
}

bool GPSRef::Connect()
{

	// Instantiations
	Client = gcnew TcpClient(IPAddress, PortNumber);
	RecvData = gcnew array<unsigned char>(256);

	// Configurations, set the read/write timeouts & buffer size
	Client->NoDelay = true;
	Client->ReceiveTimeout = 2000;//ms The number is given by Tutor 2000
	Client->SendTimeout = 500;//ms  The number is given by Tutor 2000
	Client->ReceiveBufferSize = 256; // used to be 1024
	Client->SendBufferSize = 16;


	// Get the network stream object associated with client so we 
	// can use it to read and write
	Stream = Client->GetStream();



	return true;
}

void GPSRef::GetGPSData()
{
	if (Stream->DataAvailable)
	{

		Stream->Read(RecvData, 0, RecvData->Length);
	}

		
	// Trapping the Header
	unsigned int Header = 0;
	unsigned char Data =0;
	int i = 0;
	bool headerFlag = 1;
	int Start;
	do
	{
		Data = RecvData[i++];
		Header = ((Header << 8) | Data);
		if (i == 255)
		{
			headerFlag = 0;
			break;
		}
	} while (Header != 0xaa44121c);
	if (headerFlag)
	{
		Start = i - 4;
		unsigned char* BytePtr = (unsigned char*)& NovatelGPS;
		for (int i = Start; i < Start + sizeof(GPSData); i++)
		{
			*(BytePtr + i) = RecvData[i];
		}

		CalculatedCRC = CalculateBlockCRC32(108, BytePtr);
		if (CalculatedCRC == NovatelGPS.Checksum)
		{
			Height = NovatelGPS.Height;
			Northing = NovatelGPS.Northing;
			Easting = NovatelGPS.Easting;
		}
		else
		{
			ErrorCode = 5;
			Console::WriteLine("\nError code = 5");
			Console::ReadKey();
			return;
		}
	}
	else 
	{
		ContinueFlag = 1;
	}

}

double GPSRef::GetNorthing()
{
	return this->Northing;
}

double GPSRef::GetEasting()
{
	return this->Easting;
}

double GPSRef::GetHeight()
{
	return this->Height;
}
