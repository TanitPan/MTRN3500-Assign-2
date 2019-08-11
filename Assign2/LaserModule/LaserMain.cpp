//Compile in a C++ CLR empty project
#include <SMObject.h>
#include <SMStructs.h>
#include <conio.h> // _kbhit()
#include "LiDAR.h"
#define WAIT_TIME 250

int main()
{
	// Declaring and accessing SM Object
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
		
		// If continue flag set then skip current loop
		if (MyLaser->ContinueFlag == 1) continue;
		for (int i = 0; i < MyLaser->NumRanges; i++)
		{
			LaserSMPtr->NumRanges = MyLaser->NumRanges;
			LaserSMPtr->XRange[i] = MyLaser->RangeX[i];
			LaserSMPtr->YRange[i] = MyLaser->RangeY[i];
			
		}


		// Print the received string on the screen
		MyLaser->GetXYRangeData();
		 
		Sleep(35);
	}

	Console::WriteLine("LaserMain terminated normally.");

	return 0;
}