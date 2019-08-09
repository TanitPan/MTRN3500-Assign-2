//Compile in a C++ CLR empty project
#include <SMObject.h>
#include <SMStructs.h>
//#using <System.dll>
#include "LiDAR.h"


#include <conio.h>//_kbhit()
#define WAIT_TIME 250

//using namespace System;
//using namespace System::Net::Sockets; Already include in LiDAR.h
//using namespace System::Net;
//using namespace System::Text;

//using namespace System::Threading;


int main()
{

	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));
	SMObject LaserObj(_TEXT("LaserObj"), sizeof(Laser));

	PM* PMSMPtr = nullptr;
	Laser* LaserSMPtr = nullptr;
	int WaitCount = 0;


	PMObj.SMAccess();
	if (PMObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed PMObj");
		Console::ReadKey();
		return -2;
	}

	LaserObj.SMAccess();
	if (LaserObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed LaserObj");
		Console::ReadKey();
		return -2;
	}

	PMSMPtr = (PM*)PMObj.pData;
	PMSMPtr->Shutdown.Flags.Laser = 0;

	LaserSMPtr = (Laser*)LaserObj.pData;

	
	LiDAR^ MyLaser = gcnew LiDAR("192.168.1.200", 23000);
	MyLaser->Connect();


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
				Console::WriteLine("Laser Die");
			}
			Console::WriteLine("Waitcount: " + WaitCount);
		}

		MyLaser->ContinueFlag = 0;
		MyLaser->GetLaserScan();
		
		if (MyLaser->ContinueFlag == 1) continue;
		for (int i = 0; i < MyLaser->NumRanges; i++)
		{
			LaserSMPtr->NumRanges = MyLaser->NumRanges;
			LaserSMPtr->XRange[i] = MyLaser->RangeX[i];
			LaserSMPtr->YRange[i] = MyLaser->RangeY[i];
			

			//Console::WriteLine("{0,10:F3} {1,10:F3}", LaserSMPtr->XRange[i], LaserSMPtr->YRange[i]);
		}

		//Console::WriteLine("StartAngle:{0,10:F3}  Resolution:{1,10:F3}  NumRanges:{2,10:D}", StartAngle, Resolution, NumRanges);
		//Console::WriteLine("{0,10:F3}", StartAngle);

		// Print the received string on the screen
		MyLaser->GetXYRangeData();
		 
		Sleep(35);
	}

	/*MyLaser->Stream->Close();
	MyLaser->Client->Close();*/

	Console::WriteLine("LaserMain terminated normally.");
	//Console::ReadKey();
	//Console::ReadKey();


	return 0;
}