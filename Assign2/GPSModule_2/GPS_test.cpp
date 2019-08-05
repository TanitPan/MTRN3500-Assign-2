#include "GPS.h"
//#include "GPSRef.h"
#include <SMStructs.h>
#include <SMObject.h>
#define WAIT_TIME 200 // may need some modification later

using namespace System;
using namespace System::Threading;
//using namespace System::Net;
using namespace System::Text;
//using namespace System::Net::Sockets;




int main()
{
	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));
	SMObject GPSObj(_TEXT("GPSObj"), sizeof(GPS_1));

	PM* PMSMPtr = nullptr;
	GPS_1* GPSSMPtr = nullptr;

	int WaitCount = 0;


	PMObj.SMAccess();
	if (PMObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed");
		return -2;
	}

	GPSObj.SMAccess();
	if (GPSObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed LaserObj");
		Console::ReadKey();
		return -2;
	}

	////Console::WriteLine(WaitCount);
	//PMSMPtr = (PM*)PMObj.pData;
	//PMSMPtr->Shutdown.Flags.GPS = 0;

	//GPSSMPtr = (GPS*)GPSObj.pData;

	//GPSRef^ MyGPS = gcnew GPSRef("192.168.1.200", 24000);
	//MyGPS->Connect();

	//while (!PMSMPtr->Shutdown.Flags.GPS)
	//{
	//	PMSMPtr->Heartbeats.Flags.GPS = 1;

	//	if (PMSMPtr->PMHeartbeats.Flags.GPS)
	//	{
	//		PMSMPtr->PMHeartbeats.Flags.GPS = 0;
	//		WaitCount = 0;
	//		Console::WriteLine("{0,9:F6}", WaitCount);
	//	}
	//	else
	//	{
	//		if (++WaitCount > WAIT_TIME)
	//		{
	//			Console::WriteLine(WaitCount);
	//			Console::WriteLine("GPS am dead");
	//			PMSMPtr->Shutdown.Status = 0xFF;
	//		}
	//		Console::WriteLine("Waitcount: " + WaitCount);
	//	}

	//	Console::WriteLine("{0,9:F3}", PMSMPtr->PMTimeStamp);
	//	PMSMPtr->PMTimeStamp = 0;

	//	MyGPS->ErrorCode = 0;
	//	MyGPS->ContinueFlag = 0;
	//	MyGPS->GetGPSData();
	//	if (MyGPS->ContinueFlag == 1) continue;

	//	if (!MyGPS->ErrorCode)
	//	{
	//		// Set GPS Shared memory to Northing, Easting and Height
	//		GPSSMPtr->Height = MyGPS->GetHeight();
	//		GPSSMPtr->Northing = MyGPS->GetNorthing();
	//		GPSSMPtr->Easting = MyGPS->GetEasting();


	//		Console::WriteLine("{0,10:F3} {1, 10:F3} {2, 10:F3}", GPSSMPtr->Height, GPSSMPtr->Northing, GPSSMPtr->Easting);
	//	}

	//	//if (_kbhit()) break;
	//	Sleep(20);
	//}

	//Console::ReadKey();
	Console::WriteLine("GPS terminated normally.");
	//Console::ReadKey();

	return 0;
}